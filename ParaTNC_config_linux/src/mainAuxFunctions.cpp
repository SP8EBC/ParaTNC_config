/*
 * mainAuxFunctions.cpp
 *
 *  Created on: Dec 27, 2025
 *      Author: mateusz
 */

#include "mainAuxFunctions.h"

#include "../shared/services/SrvGetVersionAndId.h"
#include "serial/Serial.h"
#include <semaphore.h>

#include "ConfigExporter.h"
#include "ConfigImporter.h"
#include "TimeTools.h"

#include <boost/program_options.hpp>

extern bool verboseLogging;

/**
 *
 * @param callsign
 * @param api_name
 * @param out
 * @return
 */
size_t main_make_filename_prefix (std::string &callsign, std::string &api_name, std::string &out)
{
	size_t total_ln = 0;

	std::string prefix = out;

	// copy station callsign and an api name to a buffer for log file prefix.
	// only after this place files might be created. prefix is 48 bytes long
	// indexes:
	// -> 0~10 	- API name				- 	11 characters
	// -> 12~17	- callsign				-	6 characters
	// -> 19 		- current time and date
	out.clear ();
	if (prefix.length () > 0) {
		out.append (prefix);
		out.push_back ('_');
	}
	out.append (api_name);
	out.push_back ('_');
	out.push_back ('_');
	out.append (callsign);
	out.push_back ('_');
	out.push_back ('_');
	TimeTools::getCurrentLocalTimeFnString (out);

	return total_ln;
}

/**
 *
 * @param did
 * @param _srvReadDid
 * @param _s
 * @param _lock
 * @param _cond1
 */
void main_readDid (const int did, SrvReadDid &_srvReadDid, Serial &_s, pthread_mutex_t &_lock,
				   sem_t &_cond1)
{
	_srvReadDid.sendRequestForDid (did);
	//_s.waitForTransmissionDone ();
	pthread_mutex_lock (&_lock);
	// wait for DID value to be received
	sem_wait (&_cond1);
	pthread_mutex_unlock (&_lock);
	const DidResponse &response = _srvReadDid.getDidResponse ();
}

std::shared_ptr<IConfigurationManager> main_readConfig (SrvGetRunningConfig &_srvRunningConfig,
														Serial &_s, pthread_mutex_t &_lock,
														sem_t &_cond1)
{
	_srvRunningConfig.sendRequest ();
	//_s.waitForTransmissionDone ();
	pthread_mutex_lock (&_lock);
	// wait for configuration to be received
	sem_wait (&_cond1);
	pthread_mutex_unlock (&_lock);
	if (_srvRunningConfig.isValidatedOk ()) {
		// create configuration manager from received data. CRC validation is done inside
		// srvRunningConfig
		std::shared_ptr<IConfigurationManager> configurationManager =
			std::make_shared<ConfigurationManager> (_srvRunningConfig.getConfigurationData ());

		return configurationManager;
	}
	else {
		throw std::runtime_error ("CRC validation failed for running configuration block!!");
	}
}

/**
 *
 * @param _srvRunningConfig
 * @param _s
 * @param _lock
 * @param _cond1
 * @param _fileNamePrefix
 * @return
 */
std::shared_ptr<IConfigurationManager> main_readConfig (SrvGetRunningConfig &_srvRunningConfig,
														Serial &_s, pthread_mutex_t &_lock,
														sem_t &_cond1, std::string _fileNamePrefix)
{
	std::string callsign; // this is required to create export filename
	std::string apiName;  // this is required to create export filename

	std::shared_ptr<IConfigurationManager> configurationManager =
		main_readConfig (_srvRunningConfig, _s, _lock, _cond1);

	IBasicConfig &basic = configurationManager->getBasicConfig ();
	IGsmConfig &gsm = configurationManager->getGsmConfig ();
	// get callsign and API station name
	basic.getCallsign (callsign);
	gsm.getApiStationName (apiName);
	// create filename prefix into fileNamePrefix
	main_make_filename_prefix (callsign, apiName, _fileNamePrefix);
	_srvRunningConfig.storeToBinaryFile (_fileNamePrefix + ".conf.bin");
	ConfigExporter exporter (configurationManager); // to text config file
	exporter.exportToFile (_fileNamePrefix + ".conf");

	return configurationManager;
}

/**
 *
 * @param _configManager
 * @param _srvReadDid
 * @param _srvEraseConfig
 * @param _srvSendStartupConfig
 * @param _batchConfig
 * @param _s
 * @param _lock
 * @param _cond1
 * @return
 */
std::shared_ptr<IConfigurationManager>
main_writeConfig (std::shared_ptr<IConfigurationManager> _configManager, SrvReadDid &_srvReadDid,
				  SrvEraseStartupConfig &_srvEraseConfig,
				  SrvSendStartupConfig &_srvSendStartupConfig, BatchConfig &_batchConfig,
				  Serial &_s, pthread_mutex_t &_lock, sem_t &_cond1)
{
	if (!_configManager) {
		_configManager = std::make_shared<ConfigurationManager> ();
	}
	ConfigImporter configImporter (_configManager);
	const bool importResult = configImporter.importFromFile (_batchConfig.configFileToWrite);
	if (!importResult) {
		throw std::runtime_error ("Configuration file malformed");
	}
	_configManager->print (IConfigurationManager::PrintVerbosity::BRIEF_SUMMARY);
	const bool configFullSet = configImporter.allSet ();
	if (configFullSet) {
		main_amendConfig (_configManager,
						  _srvReadDid,
						  _srvEraseConfig,
						  _srvSendStartupConfig,
						  _s,
						  _lock,
						  _cond1);
	}
	else {
		throw std::runtime_error ("If write-config service is used, all configuration options must "
								  "be specified in the input file. Use amend-config instead.");
	}

	return _configManager;
}

/**
 *
 * @param _configManager
 * @param _srvReadDid
 * @param _srvEraseConfig
 * @param _srvSendStartupConfig
 * @param _s
 * @param _lock
 * @param _cond1
 */
void main_amendConfig (std::shared_ptr<IConfigurationManager> _configManager,
					   SrvReadDid &_srvReadDid, SrvEraseStartupConfig &_srvEraseConfig,
					   SrvSendStartupConfig &_srvSendStartupConfig, Serial &_s,
					   pthread_mutex_t &_lock, sem_t &_cond1)
{
	if (!_configManager) {
		throw std::runtime_error ("_configManager must be set!");
	}

	// read DID 0xF000u -> config_running_pgm_counter
	_srvReadDid.sendRequestForDid (0xF000u);
	//_s.waitForTransmissionDone ();
	pthread_mutex_lock (&_lock);
	// wait for DID value to be received
	sem_wait (&_cond1);
	pthread_mutex_unlock (&_lock);
	const DidResponse &response = _srvReadDid.getDidResponse ();
	// check if DID response for id 0xF000 has correct type
	if (response.firstSize == DIDRESPONSE_DATASIZE_INT32) {
		const uint32_t configCounter = (uint32_t)((response.first.i32));
		std::cout << "I = main, old value of configCounter: " << configCounter << std::endl;
		// increase config counter to use
		_configManager->setConfigCounter (configCounter + 2);
		std::cout << "I = main, new value of configCounter: " << _configManager->getConfigCounter ()
				  << std::endl;
		const uint32_t newCrc = _configManager->calculateAndSetChecksum ();
		std::cout << "I = main, new CRC32 checksum: 0x" << std::hex << newCrc << std::endl;

		// send startup config erase request
		_srvEraseConfig.sendRequest ();
		//_s.waitForTransmissionDone ();
		pthread_mutex_lock (&_lock);
		// wait for erase to be done
		sem_wait (&_cond1);
		pthread_mutex_unlock (&_lock);
		std::cout << "I = main, erase done" << std::endl;

		auto dataToSend = _configManager->getConfigData ();
		_srvSendStartupConfig.setDataForDownload (dataToSend);
		_srvSendStartupConfig.sendRequest ();
	}
	else {
		throw std::runtime_error ("DID number 0xF000 must be a type of int32_t");
	}
}

/**
 *
 * @param argc
 * @param argv
 * @param batchConfig
 * @param portName
 * @param breakEventsLogDumpOnCrcFail
 */
void parse_commandline_args (int argc, char *argv[], BatchConfig *batchConfig,
							 std::string *portName, bool *breakEventsLogDumpOnCrcFail)
{
	boost::program_options::variables_map variablesMap;

	boost::program_options::options_description od ("");

	// SET ALL COMMANDLINE OPTIONS
	boost::program_options::options_description generalOptions ("General Options");
	boost::program_options::options_description_easy_init goInit = generalOptions.add_options ();
	goInit ("port,P",
			boost::program_options::value<std::string> (portName),
			" : Serial port used for communication");
	goInit ("valid-events,e", " : Stop dumping events log on first empty event or first crc error");
	goInit ("verbose", " : Print more things on the console");

	boost::program_options::options_description diagnosticServices ("Diagnostic Services", 120, 90);
	boost::program_options::options_description_easy_init dsInit =
		diagnosticServices.add_options ();
	dsInit ("restart", " : Restart ParaMETEO");
	dsInit ("read-did,r",
			boost::program_options::value<std::string> (&batchConfig->didToRead),
			" : Read DID (data-by-id) specified by hex in range 0000 to FFFF");
	dsInit ("monitor-did,m",
			boost::program_options::value<std::string> (&batchConfig->didToRead),
			" : Read specified DID each 2 second until this program is closed");
	dsInit ("read-config,R", " : Read running config and store it in bin and text file");
	dsInit ("write-config,W",
			boost::program_options::value<std::string> (&batchConfig->configFileToWrite),
			" : Write complete startup config from text file");
	dsInit ("amend-config,A",
			boost::program_options::value<std::string> (&batchConfig->configFileToWrite),
			" : Partially Amend config from (incomplete) text file");
	dsInit ("routine-rtc,RR", " : Call routine to set RTC to local date and time of this PC");

	od.add (generalOptions);
	od.add (diagnosticServices);

	std::cout << od << std::endl;

	// PARSE USER INPUT
	try {
		boost::program_options::store (boost::program_options::parse_command_line (argc, argv, od),
									   variablesMap);
		boost::program_options::notify (variablesMap);
	}
	catch (boost::wrapexcept<boost::program_options::unknown_option> &ex) {
		std::cout << ex.what () << std::endl << od << std::endl;
		exit (-2);
	}

	for (auto &it : variablesMap) {
		std::cout << "D = main, odVariablesMap [" << it.first << "]" << std::endl;
	}

	// CONFIGURE APPLICATION ACCORDING TO PROVIDED COMMANDLINE PARAMETERS AND OPTIONS
	if (variablesMap.count ("verbose")) {
		verboseLogging = true;
	}
	else {
		verboseLogging = false;
	}

	if (variablesMap.count ("restart")) {
		std::cout << "I = main, restart will be performed instead of normal operation!"
				  << std::endl;
		batchConfig->defaultBatch = false;
		batchConfig->monitorMode = false;
		batchConfig->performRestart = true;
	}

	if (variablesMap.count ("read-did")) {
		batchConfig->defaultBatch = false;
		batchConfig->monitorMode = false;
		batchConfig->readDid = true;
	}

	if (variablesMap.count ("monitor-did")) {
		batchConfig->defaultBatch = false;
		batchConfig->monitorMode = true;
		batchConfig->monitorDid = true;
	}

	if (variablesMap.count ("read-config")) {
		batchConfig->defaultBatch = false;
		batchConfig->monitorMode = false;
		batchConfig->readConfig = true;
	}

	if (variablesMap.count ("write-config")) {
		batchConfig->defaultBatch = false;
		batchConfig->monitorMode = false;
		batchConfig->writeConfig = true;
		batchConfig->amendConfig = false;
	}

	if (variablesMap.count ("amend-config")) {
		batchConfig->defaultBatch = false;
		batchConfig->monitorMode = false;
		batchConfig->writeConfig = false;
		batchConfig->amendConfig = true;
	}

	if (variablesMap.count ("routine-rtc")) {
		batchConfig->defaultBatch = false;
		batchConfig->monitorMode = false;
		batchConfig->routineSetRtc = true;
	}

	if (variablesMap.count ("valid-events")) {
		*breakEventsLogDumpOnCrcFail = true;
	}

	if (batchConfig->writeConfig && batchConfig->amendConfig) {
		throw std::runtime_error ("Cannot ammend and write at once!!");
	}

	if (batchConfig->monitorMode && !batchConfig->defaultBatch) {
		std::cout << "W = main, conflicting settings! DID or memory monitoring has a precendense "
					 "over the rest"
				  << std::endl;
	}
}
