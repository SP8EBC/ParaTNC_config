
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <pthread.h>
#include <serial/SerialRxBackgroundWorker.h>
#include "ProgramConfig.h"
#include "LogDumper.h"
#include "TimeTools.h"
#include "serial/Serial.h"
#include "../shared/services/SrvGetRunningConfig.h"
#include "../shared/services/SrvGetVersionAndId.h"
#include "../shared/services/SrvEraseStartupConfig.h"
#include "../shared/services/SrvSendStartupConfig.h"
#include "../shared/services/SrvReadDid.h"
#include "../shared/services/SrvReadMemory.h"
#include "../shared/services/SrvReset.h"

#include "../shared/config/decode/DecodeVer0.h"

#include "../shared/kiss_communication_service_ids.h"

#include "../shared/event_log.h"

#include <boost/program_options.hpp>

std::map<uint8_t, IService*> callbackMap;

Serial s;

SrvGetRunningConfig srvRunningConfig;
SrvGetVersionAndId srvGetVersion;
SrvEraseStartupConfig srvEraseConfig;
SrvSendStartupConfig srvSendStartupConfig(128);
SrvReadDid srvReadDid;
SrvReadMemory srvReadMemory;
SrvReset srvReset;

// Declaration of thread condition variable
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

// declaring mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

std::string str;

IConfigDecode * decode;

const uint16_t did_list[] = {
						0x1000U, 0x1001U, 0x1002U, 0x1003U, 0x1004U, 0x1100U,
						0x2003U, 0x2004U, 0x2005U, 0x2006U, 0x2007U, 0x2008U,
						0x2200U, 0x2201U, 0x2002U, 0x1504U, 0x2000U, 0x2001U,
						0x2010U, 0x2011U, 0x2012U, 0xF000U, 0xFF00U, 0xFF0FU};

uint32_t logAreaStart = 0;
uint32_t logAreaEnd = 0;
uint32_t logOldestEntry = 0;
uint32_t logNewestEntry = 0;

bool restartOnly = false;
std::string portName;

std::string fileNamePrefix;
size_t fileNamePrefixLenght = 0;

static size_t main_make_filename_prefix(std::string & callsign, std::string & api_name, char * out, size_t max_out_ln) {
	size_t total_ln = 0;

	 // copy station callsign and an api name to a buffer for log file prefix.
	 // only after this place files might be created. prefix is 48 bytes long
	 // indexes:
	 // -> 0~10 	- API name				- 	11 characters
	 // -> 12~17	- callsign				-	6 characters
	 // -> 19 		- current time and date
	 strncpy(out, api_name.c_str(), 11);
	 total_ln = strlen(out);
	 out[total_ln++] = '_';
	 strncpy(out + total_ln, callsign.c_str(), 6);
	 total_ln = strlen(out);
	 out[total_ln++] = '_';
	 TimeTools::getCurrentLocalTimeFnString(out + total_ln, max_out_ln - total_ln);
	 total_ln = strlen(out);

	 return total_ln;

}

static size_t main_make_filename_prefix(std::string & callsign, std::string & api_name, std::string & out) {
	size_t total_ln = 0;

	 // copy station callsign and an api name to a buffer for log file prefix.
	 // only after this place files might be created. prefix is 48 bytes long
	 // indexes:
	 // -> 0~10 	- API name				- 	11 characters
	 // -> 12~17	- callsign				-	6 characters
	 // -> 19 		- current time and date
	 out.clear();
	 out.append(api_name);
	 out.push_back('_');
	 out.push_back('_');
	 out.append(callsign);
	 out.push_back('_');
	 out.push_back('_');
	 TimeTools::getCurrentLocalTimeFnString(out);

	 return total_ln;
}

int main(int argc, char *argv[]) {
#ifndef _ONLY_MANUAL_CFG
	//ProgramConfig::readConfigFromFile("");
#endif

#ifdef _ONLY_MANUAL_CFG
	ProgramConfig::manualConfig();
#endif

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

	SerialRxBackgroundWorker worker(&s, callbackMap);
//	worker.backgroundTimeoutCallback = timeout_callback;

	LogDumper logDumper(srvReadMemory, cond1, worker);

	std::vector<uint8_t> test;
	test.insert(test.begin(), 0x800, 0xAB);

	boost::program_options::options_description od("Parameters");
	boost::program_options::options_description_easy_init odInit = od.add_options();
	odInit("port", boost::program_options::value<std::string>(&portName), "Serial port used for communication");
	odInit("restart", "Restart ParaMETEO");

	boost::program_options::variables_map odVariablesMap;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, od), odVariablesMap);
	boost::program_options::notify(odVariablesMap);

	bool portOpenResult = false;

	std::cout << od << std::endl;

	if (portName.length() > 1) {
    	std::cout << "I = main, opening user specified port " << portName << std::endl;
		portOpenResult = s.init(portName, B9600);
	}
	else {
		portOpenResult = s.init("/dev/ttyS0", B9600);
	}

	if (!portOpenResult) {
    	std::cout << "E = main, cannot open and configure serial port. application cannot continue!" << std::endl;
		return -1;
	}

	if (odVariablesMap.count("restart")) {
    	std::cout << "I = main, restart will be performed instead of normal operation!" << std::endl;
		restartOnly = true;
	}
	else {
		restartOnly = false;
	}

	worker.start();

	srvGetVersion.sendRequest();
	s.waitForTransmissionDone();

	// wait for software version
    pthread_mutex_lock(&lock);
    pthread_cond_wait(&cond1, &lock);
    pthread_mutex_unlock(&lock);

    if (restartOnly) {
    	srvReset.restart();
    }
    else {

		srvRunningConfig.sendRequest();
		s.waitForTransmissionDone();

		pthread_mutex_lock(&lock);
		// wait for configuration to be received
		pthread_cond_wait(&cond1, &lock);
		pthread_mutex_unlock(&lock);

		 std::string callsign;
		 std::string description;
		 std::string apiName;
		 float lat, lon;

		 decode = new DecodeVer0(srvRunningConfig.getConfigurationData());
		 decode->getCallsign(callsign);
		 decode->getDescritpion(description);
		 decode->getGsmApiStationName(apiName);

		 lon = decode->getLongitude();
		 lat = decode->getLatitude();

		 std::cout << "I = main, callsign: " << callsign << std::endl;
		 std::cout << "I = main, API station name: " << apiName << std::endl;
		 std::cout << "I = main, description: " << description << std::endl;
		 std::cout << "I = main, lon: " << lon << std::endl;
		 std::cout << "I = main, lat: " << lat << std::endl;

		 main_make_filename_prefix(callsign, apiName, fileNamePrefix);

		 std::cout << "I = main, fileNamePrefix: " << fileNamePrefix << std::endl;


		for (int i = 0; i < ((int)sizeof(did_list) / (int)(sizeof(did_list[0]))); i++) {
			std::cout << "I = main, reading DID " << std::hex << did_list[i] << std::dec << std::endl;

			srvReadDid.sendRequestForDid(did_list[i]);
			s.waitForTransmissionDone();

			pthread_mutex_lock(&lock);
			// wait for configuration to be received
			pthread_cond_wait(&cond1, &lock);
			pthread_mutex_unlock(&lock);

			if (did_list[i] == 0xFF00U) {
				// 		ENTRY(0xFF00U, main_flash_log_start, main_flash_log_end, DID_EMPTY)
				const DidResponse& response = srvReadDid.getDidResponse();
				logAreaStart = (uint32_t)response.first.i32;
				logAreaEnd =  (uint32_t)response.second.i32;
			}
			else if (did_list[i] == 0xFF0FU) {
				//		ENTRY(0xFF0FU, nvm_event_oldestFlash, nvm_event_newestFlash, DID_EMPTY)
				const DidResponse& response = srvReadDid.getDidResponse();
				logOldestEntry = (uint32_t)response.first.i32;
				logNewestEntry = (uint32_t)response.second.i32;
			}
			else {
				;
			}

		}

		std::cout << "I = main, logAreaStart at: 0x" << std::hex << logAreaStart << ", logAreaEnd at: 0x" << logAreaEnd << std::endl;
		std::cout << "I = main, logOldestEntry at: 0x" << std::hex << logOldestEntry << ", logNewestEntry at: 0x" << logNewestEntry << std::endl;

		logDumper.dumpEventsToReport(logAreaStart, logAreaEnd, fileNamePrefix + ".log");

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
	worker.terminate();

	return 0;

}


