
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <pthread.h>
#include <serial/SerialRxBackgroundWorker.h>
#include "ProgramConfig.h"
#include "serial/Serial.h"
#include "../shared/services/SrvGetRunningConfig.h"
#include "../shared/services/SrvGetVersionAndId.h"
#include "../shared/services/SrvEraseStartupConfig.h"
#include "../shared/services/SrvSendStartupConfig.h"
#include "../shared/services/SrvReadDid.h"

#include "../shared/config/decode/DecodeVer0.h"

#include "../shared/kiss_communication_service_ids.h"

std::map<uint8_t, IService*> callbackMap;

Serial s;

SrvGetRunningConfig srvRunningConfig;
SrvGetVersionAndId srvGetVersion;
SrvEraseStartupConfig srvEraseConfig;
SrvSendStartupConfig srvSendStartupConfig(128);
SrvReadDid srvReadDid;

// Declaration of thread condition variable
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

// declaring mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

std::string str;

IConfigDecode * decode;

const uint16_t did_list[] = {
						0x1000U, 0x1001U, 0x1002U, 0x1003U, 0x1004U, 0x1100U,
						0x1500U, 0x1501U, 0x1502U, 0x1503U, 0x1504U, 0x1505U,
						0x2000U, 0x2001U, 0x2002U, 0x2003U, 0x2004U, 0x2005U,
						0x2006U, 0x2007U, 0x2222U, 0x5555U};

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

	srvGetVersion.setConditionVariable(&cond1);
	srvRunningConfig.setConditionVariable(&cond1);
	srvEraseConfig.setConditionVariable(&cond1);
	srvReadDid.setConditionVariable(&cond1);

	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_RUNNING_CONFIG, &srvRunningConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_VERSION_AND_ID, &srvGetVersion));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_ERASE_STARTUP_CFG_RESP, &srvEraseConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_PROGRAM_STARTUP_CFG_RESP, &srvSendStartupConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_READ_DID_RESP, &srvReadDid));

	SerialRxBackgroundWorker worker(&s, callbackMap);

	std::vector<uint8_t> test;
	test.insert(test.begin(), 0x800, 0xAB);


	if (argc > 1) {
		s.init(argv[1], B9600);
	}
	else {
		s.init("/dev/ttyS0", B9600);
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
    }

	srvRunningConfig.sendRequest();
	s.waitForTransmissionDone();

    pthread_mutex_lock(&lock);
    // wait for configuration to be received
    pthread_cond_wait(&cond1, &lock);
    pthread_mutex_unlock(&lock);

    srvRunningConfig.storeToBinaryFile("config.bin");

    std::string callsign;
    std::string description;
    float lat, lon;

	decode = new DecodeVer0(srvRunningConfig.getConfigurationData());
	decode->getCallsign(callsign);
	decode->getDescritpion(description);
	lon = decode->getLongitude();
	lat = decode->getLatitude();

	std::cout << "I = main, callsign: " << callsign << std::endl;
	std::cout << "I = main, description: " << description << std::endl;
	std::cout << "I = main, lon: " << lon << std::endl;
	std::cout << "I = main, lat: " << lat << std::endl;


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


