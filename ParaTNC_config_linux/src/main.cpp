#include "mainAuxFunctions.h"

#include "../shared/services/SrvEraseStartupConfig.h"
#include "../shared/services/SrvGetRunningConfig.h"
#include "../shared/services/SrvGetVersionAndId.h"
#include "../shared/services/SrvReadDid.h"
#include "../shared/services/SrvReadMemory.h"
#include "../shared/services/SrvReset.h"
#include "../shared/services/SrvRoutineControl.h"
#include "../shared/services/SrvSendStartupConfig.h"
#include "BatchConfig_t.h"
#include "ConfigExporter.h"
#include "ConfigImporter.h"
#include "LogDumper.h"
#include "Routines.hpp"
#include "TimeTools.h"
#include "did_decoder/DescriptionIniFileReader.h"
#include "did_decoder/DidDecoder.h"
#include "serial/Serial.h"
#include <iomanip>
#include <iostream>
#include <memory>
#include <semaphore.h>
#include <serial/SerialPromptUserForPort.h>
#include <serial/SerialRxBackgroundWorker.h>
#include <vector>

#include "../shared/config/ConfigVer0.h"

#include "../shared/kiss_communication_service_ids.h"

#include "../shared/event_log.h"

#define PRINT_RAW_DID (didPrintResponse || !didIniAvailable)

std::map<uint8_t, IService *> callbackMap;

Serial s;

SrvGetRunningConfig srvRunningConfig;
SrvGetVersionAndId srvGetVersion;
SrvEraseStartupConfig srvEraseConfig;
SrvSendStartupConfig srvSendStartupConfig (128);
SrvReadDid srvReadDid;
SrvReadMemory srvReadMemory;
SrvReset srvReset;
SrvRoutineControl srvRoutineControl;

// Declaration of thread condition variable
sem_t cond1;

// declaring mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

std::string str;

std::shared_ptr<IConfigurationManager> configManager;

/**
 * Default DID list used if no INI description file is available
 */
const uint16_t fixedDidList[] = {0x1000U, 0x1001U, 0x1002U, 0x1003U, 0x1004U, 0x1100U,
								 0x2003U, 0x2004U, 0x2005U, 0x2006U, 0x2007U, 0x2008U,
								 0x2200U, 0x2201U, 0x2002U, 0x1504U, 0x2000U, 0x2001U,
								 0x2010U, 0x2011U, 0x2012U, 0xF000U, 0xFF00U, 0xFF0FU};

/**
 * List of DIDs to be read in default batch. Filled with IDs from INI description file
 * or initialized with array @link{fixedDidList} if this file is not available
 */
std::vector<uint16_t> didList;

uint32_t logAreaStart = 0;
uint32_t logAreaEnd = 0;
uint32_t logOldestEntry = 0;
uint32_t logNewestEntry = 0;

std::string fileNamePrefix;
size_t fileNamePrefixLenght = 0;

bool verboseLogging;

static void nrc_callback (uint16_t nrc)
{
	// exit (nrc);
	sem_post (&cond1);
}

static void timeout_callback (void)
{
	sem_post (&cond1);
}

void routine_result_callback (RoutineControlResult result)
{
	std::cout << "I = routine_result_callback, routineId " << result.routineId
			  << ", subfunction: " << result.subfunction << ", resultCode: " << result.resultCode
			  << std::endl;

	sem_post (&cond1);
}

int main (int argc, char *argv[])
{
	std::string portName;
	BatchConfig batchConfig;

	// clang-format off
	TimeTools::initBoostTimezones();

	srvRunningConfig.setSerialContext(&s);
	srvGetVersion.setSerialContext(&s);
	srvEraseConfig.setSerialContext(&s);
	srvSendStartupConfig.setSerialContext(&s);
	srvReadDid.setSerialContext(&s);
	srvReadMemory.setSerialContext(&s);
	srvReset.setSerialContext(&s);
	srvRoutineControl.setSerialContext(&s);

	srvGetVersion.setConditionVariable(&cond1);
	srvRunningConfig.setConditionVariable(&cond1);
	srvEraseConfig.setConditionVariable(&cond1);
	srvReadDid.setConditionVariable(&cond1);
	srvReadMemory.setConditionVariable(&cond1);
	srvReset.setConditionVariable(&cond1);
	srvRoutineControl.setConditionVariable(&cond1);

	// put all handlers for diagnostics services into a callback map.
	// this map is later used by @link{SerialRxBackgroundWorker} to know
	// what to call on incoming diagnostic services response
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_RUNNING_CONFIG, &srvRunningConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_VERSION_AND_ID, &srvGetVersion));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_ERASE_STARTUP_CFG_RESP, &srvEraseConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_PROGRAM_STARTUP_CFG_RESP, &srvSendStartupConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_READ_DID_RESP, &srvReadDid));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_READ_MEM_ADDR_RESP, &srvReadMemory));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_RESTART, &srvReset));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_ROUTINE_CONTROL_RESP, &srvRoutineControl));

	bool breakEventsLogDumpOnCrcFail = false;
	bool didPrintResponse = true;

	SerialRxBackgroundWorker worker(&s, callbackMap, nrc_callback);
	worker.backgroundTimeoutCallback = timeout_callback;

	LogDumper logDumper(srvReadMemory, cond1, worker);

	batchConfig.defaultBatch = true;

	// parsing all commandline arguments
	parse_commandline_args(argc, argv, &batchConfig, &portName, &breakEventsLogDumpOnCrcFail);

	// creating an instance of wrapper class, which implements a specifics of each
	// diagnostics routine and exposes convinent api to use each of them, instead of "raw"
	// calls to Routine Control diagnostics service
	Routines routines(srvRoutineControl, srvReadDid);

	DescriptionIniFileReader didIniReader("did-description.ini");
	const bool didIniAvailable = didIniReader.parse();
	const std::map<uint16_t, DidDescription> &didDescription = didIniReader.getDescriptions();

	DidDecoder didDecoder(didDescription);

	if (didIniAvailable) {
		std::cout << "I = main, DID description file created " << didIniReader.getCreationDate()
				<< ", having " << didDescription.size() << " definitions" << std::endl;

		std::cout << "I = main, DID description file header info: " << didIniReader.getHeaderDescription() <<  std::endl;

		didList = didIniReader.getDidList();
	}
	else {
		// if INI description file is not available create list from hardcoded array
		didList = std::vector<uint16_t>(std::begin(fixedDidList), std::end(fixedDidList));
	}

	bool portOpenResult = false;

	if (portName.length () > 1) {
		std::cout << "I = main, opening user specified port " << portName << std::endl;
		portOpenResult = s.init (portName, 9600);
	}
	else {
		std::string port = SerialPromptUserForPort::promptForSerial ();
		portOpenResult = s.init (port, 9600);
	}

	if (!portOpenResult) {
		std::cout << "E = main, cannot open and configure serial port. application cannot continue!"
				  << std::endl;
		return -1;
	}

	worker.start ();

	// clang-format on
	// inverted logic to push default batch to the end
	if (batchConfig.monitorMode) {
		const int did = strtol (batchConfig.didToRead.c_str (), NULL, 16);
		didPrintResponse = !didIniReader.hasDescriptionForDid (did);

		while (true) {
			srvReadDid.sendRequestForDid (did, PRINT_RAW_DID);

			pthread_mutex_lock (&lock);
			// wait for configuration to be received
			sem_wait (&cond1);
			pthread_mutex_unlock (&lock);

			if (!PRINT_RAW_DID) {
				didDecoder.decodeAndPrintDid (did, srvReadDid.getDidResponse ());
				didDecoder.m_printDidNameDescription = false;
			}

			sleep (1);
		}
	}
	else if (!batchConfig.defaultBatch && !batchConfig.monitorMode) {
		// exec diagnostic services in order
		if (batchConfig.routineSetRtc) {
			routines.setRtcToLocalDateTime ();
		}
		if (batchConfig.readDid) {
			const int did = strtol (batchConfig.didToRead.c_str (), NULL, 16);
			didPrintResponse = !didIniReader.hasDescriptionForDid (did);
			std::cout << "D = main, reading DID: 0x" << std::hex << did << std::endl;

			main_readDid (did, srvReadDid, s, lock, cond1, PRINT_RAW_DID);
			if (!PRINT_RAW_DID) {
				didDecoder.decodeAndPrintDid (did, srvReadDid.getDidResponse ());
			}
			std::cout << "D = main, did has been read" << std::endl;
		}
		if (batchConfig.readConfig) {
			configManager = main_readConfig (srvRunningConfig, s, lock, cond1, fileNamePrefix);
		}
		if (batchConfig.writeConfig) {
			configManager = main_writeConfig (configManager,
											  srvReadDid,
											  srvEraseConfig,
											  srvSendStartupConfig,
											  batchConfig,
											  s,
											  lock,
											  cond1);
		}
		if (batchConfig.amendConfig) {
			configManager = main_readConfig (srvRunningConfig, s, lock, cond1);

			ConfigImporter configImporter (configManager);

			const bool importResult = configImporter.importFromFile (batchConfig.configFileToWrite);
			if (!importResult) {
				throw std::runtime_error ("Configuration file malformed");
			}

			main_amendConfig (configManager,
							  srvReadDid,
							  srvEraseConfig,
							  srvSendStartupConfig,
							  s,
							  lock,
							  cond1);
		}
		if (batchConfig.performRestart) {
			srvReset.restart ();
		}
	}
	else {
		srvGetVersion.sendRequest ();

		// wait for software version
		pthread_mutex_lock (&lock);
		sem_wait (&cond1);
		pthread_mutex_unlock (&lock);

		srvRunningConfig.sendRequest ();

		pthread_mutex_lock (&lock);
		// wait for configuration to be received
		sem_wait (&cond1);
		pthread_mutex_unlock (&lock);

		std::string callsign;
		std::string apiName;

		configManager =
			std::make_shared<ConfigurationManager> (srvRunningConfig.getConfigurationData ());

		IBasicConfig &basic = configManager->getBasicConfig ();
		IGsmConfig &gsm = configManager->getGsmConfig ();

		basic.getCallsign (callsign);
		gsm.getApiStationName (apiName);

		main_make_filename_prefix (callsign, apiName, fileNamePrefix);

		std::cout << "I = main, fileNamePrefix: " << fileNamePrefix << std::endl;

		srvRunningConfig.storeToBinaryFile (fileNamePrefix + ".conf.bin");
		ConfigExporter exporter (configManager);
		exporter.exportToFile (fileNamePrefix + ".conf");

		for (size_t i = 0; i < didList.size (); i++) {
			std::cout << "I = main, reading DID " << std::hex << didList[i] << std::dec
					  << std::endl;

			didPrintResponse = !didIniReader.hasDescriptionForDid (didList[i]);
			srvReadDid.sendRequestForDid (didList[i], PRINT_RAW_DID);

			pthread_mutex_lock (&lock);
			// wait for configuration to be received
			sem_wait (&cond1);
			pthread_mutex_unlock (&lock);

			try {
				if (!PRINT_RAW_DID) {
					didDecoder.decodeAndPrintDid (didList[i], srvReadDid.getDidResponse ());
				}

				if (didList[i] == 0xFF00U) {
					// 		ENTRY(0xFF00U, main_flash_log_start, main_flash_log_end, DID_EMPTY)
					const DidResponse &response = srvReadDid.getDidResponse ();
					logAreaStart = (uint32_t)response.first.i32;
					logAreaEnd = (uint32_t)response.second.i32;
				}
				else if (didList[i] == 0xFF0FU) {
					//		ENTRY(0xFF0FU, nvm_event_oldestFlash, nvm_event_newestFlash, DID_EMPTY)
					const DidResponse &response = srvReadDid.getDidResponse ();
					logOldestEntry = (uint32_t)response.first.i32;
					logNewestEntry = (uint32_t)response.second.i32;
				}
				else {
					;
				}
			}
			catch (std::runtime_error &er) {
				std::cout << "E = main, std::runtime_error thrown while parsing response for DID 0x"
						  << std::hex << didList[i] << std::dec << ": " <<  er.what () << std::endl;
			}
		}

		std::cout << "I = main, logAreaStart at: 0x" << std::hex << logAreaStart
				  << ", logAreaEnd at: 0x" << logAreaEnd << std::endl;
		std::cout << "I = main, logOldestEntry at: 0x" << std::hex << logOldestEntry
				  << ", logNewestEntry at: 0x" << logNewestEntry << std::endl;

		logDumper.dumpEventsToReport (logAreaStart,
									  logAreaEnd,
									  fileNamePrefix + ".log",
									  breakEventsLogDumpOnCrcFail);
	}
	worker.terminate ();

	return 0;
}
