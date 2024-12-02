
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <pthread.h>
#include <serial/SerialRxBackgroundWorker.h>
#include "ProgramConfig.h"
#include "LogDumper.h"
#include "serial/Serial.h"
#include "../shared/services/SrvGetRunningConfig.h"
#include "../shared/services/SrvGetVersionAndId.h"
#include "../shared/services/SrvEraseStartupConfig.h"
#include "../shared/services/SrvSendStartupConfig.h"
#include "../shared/services/SrvReadDid.h"
#include "../shared/services/SrvReadMemory.h"

#include "../shared/config/decode/DecodeVer0.h"

#include "../shared/kiss_communication_service_ids.h"

#include "../shared/event_log.h"

std::map<uint8_t, IService*> callbackMap;

Serial s;

SrvGetRunningConfig srvRunningConfig;
SrvGetVersionAndId srvGetVersion;
SrvEraseStartupConfig srvEraseConfig;
SrvSendStartupConfig srvSendStartupConfig(128);
SrvReadDid srvReadDid;
SrvReadMemory srvReadMemory;

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


int main(int argc, char *argv[]) {
#ifndef _ONLY_MANUAL_CFG
	//ProgramConfig::readConfigFromFile("");
#endif

#ifdef _ONLY_MANUAL_CFG
	ProgramConfig::manualConfig();
#endif

	//s = std::make_shared<Serial>();

	srvRunningConfig.setSerialContext(&s);
	srvGetVersion.setSerialContext(&s);
	srvEraseConfig.setSerialContext(&s);
	srvSendStartupConfig.setSerialContext(&s);
	srvReadDid.setSerialContext(&s);
	srvReadMemory.setSerialContext(&s);

	srvGetVersion.setConditionVariable(&cond1);
	srvRunningConfig.setConditionVariable(&cond1);
	srvEraseConfig.setConditionVariable(&cond1);
	srvReadDid.setConditionVariable(&cond1);
	srvReadMemory.setConditionVariable(&cond1);

	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_RUNNING_CONFIG, &srvRunningConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_VERSION_AND_ID, &srvGetVersion));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_ERASE_STARTUP_CFG_RESP, &srvEraseConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_PROGRAM_STARTUP_CFG_RESP, &srvSendStartupConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_READ_DID_RESP, &srvReadDid));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_READ_MEM_ADDR_RESP, &srvReadMemory));

	SerialRxBackgroundWorker worker(&s, callbackMap);
//	worker.backgroundTimeoutCallback = timeout_callback;

	LogDumper logDumper(srvReadMemory, cond1, worker);

	std::vector<uint8_t> test;
	test.insert(test.begin(), 0x800, 0xAB);

	bool portOpenResult = false;

	if (argc > 1) {
		portOpenResult = s.init(argv[1], B9600);
	}
	else {
		portOpenResult = s.init("/dev/ttyS0", B9600);
	}

	if (!portOpenResult) {
		return -1;
	}

	worker.start();
	//worker.waitForStartup();
//	srvRunningConfig.sendRequest();
//	s->waitForTransmissionDone();
	srvGetVersion.sendRequest();
	s.waitForTransmissionDone();

	// wait for software version
    pthread_mutex_lock(&lock);
    pthread_cond_wait(&cond1, &lock);
    pthread_mutex_unlock(&lock);

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

	logDumper.dumpEventsToReport(logAreaStart, logAreaEnd, "234");

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
	worker.terminate();

	return 0;

}


