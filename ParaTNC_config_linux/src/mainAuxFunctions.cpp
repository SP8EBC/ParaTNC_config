/*
 * mainAuxFunctions.cpp
 *
 *  Created on: Dec 27, 2025
 *      Author: mateusz
 */

#include "mainAuxFunctions.h"

#include "../shared/services/SrvGetVersionAndId.h"
#include "serial/Serial.h"
#include <pthread.h>

#include "ConfigExporter.h"
#include "ConfigImporter.h"
#include "TimeTools.h"

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
	if (prefix.length() > 0)
	{
		out.append(prefix);
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
				   pthread_cond_t &_cond1)
{
	_srvReadDid.sendRequestForDid (did);
	_s.waitForTransmissionDone ();
	pthread_mutex_lock (&_lock);
	// wait for DID value to be received
	pthread_cond_wait (&_cond1, &_lock);
	pthread_mutex_unlock (&_lock);
	const DidResponse &response = _srvReadDid.getDidResponse ();
}

std::shared_ptr<IConfigurationManager> main_readConfig (SrvGetRunningConfig &_srvRunningConfig,
														Serial &_s, pthread_mutex_t &_lock,
														pthread_cond_t &_cond1)
{
	_srvRunningConfig.sendRequest ();
	_s.waitForTransmissionDone ();
	pthread_mutex_lock (&_lock);
	// wait for configuration to be received
	pthread_cond_wait (&_cond1, &_lock);
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
														pthread_cond_t &_cond1,
														std::string _fileNamePrefix)
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
				  Serial &_s, pthread_mutex_t &_lock, pthread_cond_t &_cond1)
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
					   pthread_mutex_t &_lock, pthread_cond_t &_cond1)
{
	if (!_configManager) {
		throw std::runtime_error ("_configManager must be set!");
	}

	// read DID 0xF000u -> config_running_pgm_counter
	_srvReadDid.sendRequestForDid (0xF000u);
	_s.waitForTransmissionDone ();
	pthread_mutex_lock (&_lock);
	// wait for DID value to be received
	pthread_cond_wait (&_cond1, &_lock);
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
		_s.waitForTransmissionDone ();
		pthread_mutex_lock (&_lock);
		// wait for erase to be done
		pthread_cond_wait (&_cond1, &_lock);
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
