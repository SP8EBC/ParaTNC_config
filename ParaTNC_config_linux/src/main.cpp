
#include "../shared/services/SrvEraseStartupConfig.h"
#include "../shared/services/SrvGetRunningConfig.h"
#include "../shared/services/SrvGetVersionAndId.h"
#include "../shared/services/SrvReadDid.h"
#include "../shared/services/SrvReadMemory.h"
#include "../shared/services/SrvReset.h"
#include "../shared/services/SrvSendStartupConfig.h"
#include "LogDumper.h"
#include "ProgramConfig.h"
#include "TimeTools.h"
#include "ConfigImporter.h"
#include "ConfigExporter.h"
#include "serial/Serial.h"
#include <iomanip>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <serial/SerialRxBackgroundWorker.h>
#include <vector>

#include "../shared/config/ConfigVer0.h"

#include "../shared/kiss_communication_service_ids.h"

#include "../shared/event_log.h"

#include <boost/program_options.hpp>

typedef struct BatchConfig {
	BatchConfig ()
		: defaultBatch (false), monitorMode (false), performRestart (false), readDid (false),
		  monitorDid (false), didToRead ()
	{
	}

	// no diagnostic services has been selected, perform default batch
	bool defaultBatch;
	bool monitorMode;

	bool performRestart;

	bool readDid;
	bool monitorDid;
	std::string didToRead;
} BatchConfig;

std::map<uint8_t, IService *> callbackMap;

Serial s;

SrvGetRunningConfig srvRunningConfig;
SrvGetVersionAndId srvGetVersion;
SrvEraseStartupConfig srvEraseConfig;
SrvSendStartupConfig srvSendStartupConfig (128);
SrvReadDid srvReadDid;
SrvReadMemory srvReadMemory;
SrvReset srvReset;

// Declaration of thread condition variable
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

// declaring mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

std::string str;

std::shared_ptr<IConfigurationManager> configManager;

const uint16_t did_list[] = {0x1000U, 0x1001U, 0x1002U, 0x1003U, 0x1004U, 0x1100U,
							 0x2003U, 0x2004U, 0x2005U, 0x2006U, 0x2007U, 0x2008U,
							 0x2200U, 0x2201U, 0x2002U, 0x1504U, 0x2000U, 0x2001U,
							 0x2010U, 0x2011U, 0x2012U, 0xF000U, 0xFF00U, 0xFF0FU};

uint32_t logAreaStart = 0;
uint32_t logAreaEnd = 0;
uint32_t logOldestEntry = 0;
uint32_t logNewestEntry = 0;

std::string portName;

std::string fileNamePrefix;
size_t fileNamePrefixLenght = 0;

BatchConfig batchConfig;

static size_t main_make_filename_prefix (std::string &callsign, std::string &api_name, char *out,
										 size_t max_out_ln)
{
	size_t total_ln = 0;

	// copy station callsign and an api name to a buffer for log file prefix.
	// only after this place files might be created. prefix is 48 bytes long
	// indexes:
	// -> 0~10 	- API name				- 	11 characters
	// -> 12~17	- callsign				-	6 characters
	// -> 19 		- current time and date
	strncpy (out, api_name.c_str (), 11);
	total_ln = strlen (out);
	out[total_ln++] = '_';
	strncpy (out + total_ln, callsign.c_str (), 6);
	total_ln = strlen (out);
	out[total_ln++] = '_';
	TimeTools::getCurrentLocalTimeFnString (out + total_ln, max_out_ln - total_ln);
	total_ln = strlen (out);

	return total_ln;
}

static size_t main_make_filename_prefix (std::string &callsign, std::string &api_name,
										 std::string &out)
{
	size_t total_ln = 0;

	// copy station callsign and an api name to a buffer for log file prefix.
	// only after this place files might be created. prefix is 48 bytes long
	// indexes:
	// -> 0~10 	- API name				- 	11 characters
	// -> 12~17	- callsign				-	6 characters
	// -> 19 		- current time and date
	out.clear ();
	out.append (api_name);
	out.push_back ('_');
	out.push_back ('_');
	out.append (callsign);
	out.push_back ('_');
	out.push_back ('_');
	TimeTools::getCurrentLocalTimeFnString (out);

	return total_ln;
}

static void nrc_callback (uint16_t nrc)
{
	exit (nrc);
}

static void timeout_callback (void)
{
	pthread_cond_signal (&cond1);
}

int main (int argc, char *argv[])
{
#ifndef _ONLY_MANUAL_CFG
	// ProgramConfig::readConfigFromFile("");
#endif

#ifdef _ONLY_MANUAL_CFG
	ProgramConfig::manualConfig ();
#endif
	// clang-format off
	TimeTools::initBoostTimezones();

	srvRunningConfig.setSerialContext(&s);
	srvGetVersion.setSerialContext(&s);
	srvEraseConfig.setSerialContext(&s);
	srvSendStartupConfig.setSerialContext(&s);
	srvReadDid.setSerialContext(&s);
	srvReadMemory.setSerialContext(&s);
	srvReset.setSerialContext(&s);

	srvGetVersion.setConditionVariable(&cond1);
	srvRunningConfig.setConditionVariable(&cond1);
	srvEraseConfig.setConditionVariable(&cond1);
	srvReadDid.setConditionVariable(&cond1);
	srvReadMemory.setConditionVariable(&cond1);
	srvReset.setConditionVariable(&cond1);

	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_RUNNING_CONFIG, &srvRunningConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_VERSION_AND_ID, &srvGetVersion));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_ERASE_STARTUP_CFG_RESP, &srvEraseConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_PROGRAM_STARTUP_CFG_RESP, &srvSendStartupConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_READ_DID_RESP, &srvReadDid));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_READ_MEM_ADDR_RESP, &srvReadMemory));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_RESTART, &srvReset));

	SerialRxBackgroundWorker worker(&s, callbackMap, nrc_callback);
	worker.backgroundTimeoutCallback = timeout_callback;

	LogDumper logDumper(srvReadMemory, cond1, worker);

	batchConfig.defaultBatch = true;

	std::vector<uint8_t> test;
	test.insert(test.begin(), 0x800, 0xAB);

	boost::program_options::options_description od("");

	boost::program_options::options_description generalOptions("General Options");
	boost::program_options::options_description_easy_init goInit = generalOptions.add_options();
	goInit("port,P", boost::program_options::value<std::string>(&portName), " : Serial port used for communication");

	boost::program_options::options_description diagnosticServices("Diagnostic Services", 120, 90);
	boost::program_options::options_description_easy_init dsInit = diagnosticServices.add_options();
	dsInit("restart", " : Restart ParaMETEO");
	dsInit("read-did,r", boost::program_options::value<std::string>(&batchConfig.didToRead), " : Read DID (data-by-id) specified by hex in range 0000 to FFFF");
	dsInit("monitor-did,m", boost::program_options::value<std::string>(&batchConfig.didToRead), " : Read specified DID each 2 second until this program is closed");

	od.add(generalOptions);
	od.add(diagnosticServices);

	boost::program_options::variables_map odVariablesMap;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, od), odVariablesMap);
	boost::program_options::notify(odVariablesMap);

	bool portOpenResult = false;

	std::cout << od << std::endl;
	// clang-format on

	for (auto &it : odVariablesMap) {
		std::cout << "D = main, odVariablesMap [" << it.first << "]" << std::endl;
	}

	if (portName.length () > 1) {
		std::cout << "I = main, opening user specified port " << portName << std::endl;
		portOpenResult = s.init (portName, B9600);
	}
	else {
		portOpenResult = s.init ("/dev/ttyS0", B9600);
	}

	if (!portOpenResult) {
		std::cout << "E = main, cannot open and configure serial port. application cannot continue!"
				  << std::endl;
		return -1;
	}

	if (odVariablesMap.count ("restart")) {
		std::cout << "I = main, restart will be performed instead of normal operation!"
				  << std::endl;
		batchConfig.defaultBatch = false;
		batchConfig.monitorMode = false;
		batchConfig.performRestart = true;
	}

	if (odVariablesMap.count ("read-did")) {
		batchConfig.defaultBatch = false;
		batchConfig.monitorMode = false;
		batchConfig.readDid = true;
	}

	if (odVariablesMap.count ("monitor-did")) {
		batchConfig.defaultBatch = false;
		batchConfig.monitorMode = true;
		batchConfig.monitorDid = true;
	}

	if (batchConfig.monitorMode && !batchConfig.defaultBatch) {
		std::cout << "W = main, conflicting settings! DID or memory monitoring has a precendense "
					 "over the rest"
				  << std::endl;
	}

	worker.start ();

	// inverted logic to push default batch to the end
	if (batchConfig.monitorMode) {
		const int did = strtol (batchConfig.didToRead.c_str (), NULL, 16);

		while (true) {
			srvReadDid.sendRequestForDid (did);
			s.waitForTransmissionDone ();

			pthread_mutex_lock (&lock);
			// wait for configuration to be received
			pthread_cond_wait (&cond1, &lock);
			pthread_mutex_unlock (&lock);

			sleep (1);
		}
	}
	else if (!batchConfig.defaultBatch && !batchConfig.monitorMode) {
		if (batchConfig.readDid) {
			const int did = strtol (batchConfig.didToRead.c_str (), NULL, 16);
			std::cout << "D = main, reading DID: 0x" << std::hex << did << std::endl;

			srvReadDid.sendRequestForDid (did);
			s.waitForTransmissionDone ();

			pthread_mutex_lock (&lock);
			// wait for configuration to be received
			pthread_cond_wait (&cond1, &lock);
			pthread_mutex_unlock (&lock);

			const DidResponse &response = srvReadDid.getDidResponse ();
			std::cout << "D = main, did has been read" << std::endl;
		}
		if (batchConfig.performRestart) {
			srvReset.restart ();
		}
	}
	else {
		srvGetVersion.sendRequest ();
		s.waitForTransmissionDone ();

		// wait for software version
		pthread_mutex_lock (&lock);
		pthread_cond_wait (&cond1, &lock);
		pthread_mutex_unlock (&lock);

		srvRunningConfig.sendRequest ();
		s.waitForTransmissionDone ();

		pthread_mutex_lock (&lock);
		// wait for configuration to be received
		pthread_cond_wait (&cond1, &lock);
		pthread_mutex_unlock (&lock);

		std::string callsign;
		std::string description;
		std::string apiName;
		float lat, lon;

		//configManager = new ConfigurationManager(srvRunningConfig.getConfigurationData ());
		configManager = std::make_shared<ConfigurationManager>(srvRunningConfig.getConfigurationData ());

	    IBasicConfig& basic = configManager->getBasicConfig();
//	    IModeConfig& mode = configManager->getModeConfig();
//	    ISourceConfig& source = configManager->getSourceConfig();
//	    IUmbConfig& umb = configManager->getUmbConfig();
//	    IRtuConfig& rtu = configManager->getRtuConfig();
	    IGsmConfig& gsm = configManager->getGsmConfig();

//		decode = new ConfigVer0 (srvRunningConfig.getConfigurationData ());
	    basic.getCallsign (callsign);
	    basic.getComment (description);
	    gsm.getApiStationName (apiName);

		lon = basic.getLongitude ();
		lat = basic.getLatitude ();

		std::cout << "I = main, callsign: " << callsign << std::endl;
		std::cout << "I = main, API station name: " << apiName << std::endl;
		std::cout << "I = main, description: " << description << std::endl;
		std::cout << "I = main, lon: " << lon << std::endl;
		std::cout << "I = main, lat: " << lat << std::endl;

		main_make_filename_prefix (callsign, apiName, fileNamePrefix);

		std::cout << "I = main, fileNamePrefix: " << fileNamePrefix << std::endl;

		srvRunningConfig.storeToBinaryFile (fileNamePrefix + ".conf.bin");
		ConfigExporter exporter(configManager);
		exporter.exportToFile(fileNamePrefix + ".conf");

		for (int i = 0; i < ((int)sizeof (did_list) / (int)(sizeof (did_list[0]))); i++) {
			std::cout << "I = main, reading DID " << std::hex << did_list[i] << std::dec
					  << std::endl;

			srvReadDid.sendRequestForDid (did_list[i]);
			s.waitForTransmissionDone ();

			pthread_mutex_lock (&lock);
			// wait for configuration to be received
			pthread_cond_wait (&cond1, &lock);
			pthread_mutex_unlock (&lock);

			if (did_list[i] == 0xFF00U) {
				// 		ENTRY(0xFF00U, main_flash_log_start, main_flash_log_end, DID_EMPTY)
				const DidResponse &response = srvReadDid.getDidResponse ();
				logAreaStart = (uint32_t)response.first.i32;
				logAreaEnd = (uint32_t)response.second.i32;
			}
			else if (did_list[i] == 0xFF0FU) {
				//		ENTRY(0xFF0FU, nvm_event_oldestFlash, nvm_event_newestFlash, DID_EMPTY)
				const DidResponse &response = srvReadDid.getDidResponse ();
				logOldestEntry = (uint32_t)response.first.i32;
				logNewestEntry = (uint32_t)response.second.i32;
			}
			else {
				;
			}
		}

		std::cout << "I = main, logAreaStart at: 0x" << std::hex << logAreaStart
				  << ", logAreaEnd at: 0x" << logAreaEnd << std::endl;
		std::cout << "I = main, logOldestEntry at: 0x" << std::hex << logOldestEntry
				  << ", logNewestEntry at: 0x" << logNewestEntry << std::endl;

		logDumper.dumpEventsToReport (logAreaStart, logAreaEnd, fileNamePrefix + ".log");

		////////////////////////

		// srvRunningConfig.sendRequest();
		// s.waitForTransmissionDone();

		// pthread_mutex_lock(&lock);
		// // wait for configuration to be received
		// pthread_cond_wait(&cond1, &lock);
		// pthread_mutex_unlock(&lock);

		// srvRunningConfig.storeToBinaryFile("config.bin");

		// std::string callsign;
		// std::string description;
		// float lat, lon;

		// decode = new DecodeVer0(srvRunningConfig.getConfigurationData());
		// decode->getCallsign(callsign);
		// decode->getDescritpion(description);
		// lon = decode->getLongitude();
		// lat = decode->getLatitude();

		// std::cout << "I = main, callsign: " << callsign << std::endl;
		// std::cout << "I = main, description: " << description << std::endl;
		// std::cout << "I = main, lon: " << lon << std::endl;
		// std::cout << "I = main, lat: " << lat << std::endl;

		//////////////////////////

		//	srvEraseConfig.sendRequest();
		//	s.waitForTransmissionDone();
		//
		//    pthread_mutex_lock(&lock);
		//    // wait for erase to be done
		//    pthread_cond_wait(&cond1, &lock);
		//    pthread_mutex_unlock(&lock);
		//
		//    std::cout << "erase done" << std::endl;
		//
		//    srvSendStartupConfig.setDataForDownload(test);
		//    srvSendStartupConfig.sendRequest();

		//    DecodeVer0 decode(srvRunningConfig.getConfigurationData());
		//
		//    decode.getDescritpion(str);
		//    std::cout << str << std::endl;

		//	s->transmitKissFrame(pointerTxTest);
		//	s->receiveKissFrame(pointerRxTest);
	}
	worker.terminate ();

	return 0;
}
