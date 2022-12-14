
#include <iostream>
#include <vector>
#include <memory>
#include <pthread.h>
#include "ProgramConfig.h"
#include "serial/Serial.h"
#include "serial/SerialWorker.h"

#include "services/ServicesIds.h"
#include "services/SrvGetRunningConfig.h"
#include "services/SrvGetVersionAndId.h"
#include "services/SrvEraseStartupConfig.h"
#include "services/SrvSendStartupConfig.h"

#include "config/decode/DecodeVer0.h"

std::map<uint8_t, IService*> callbackMap;

std::shared_ptr<Serial> s;

SrvGetRunningConfig srvRunningConfig;
SrvGetVersionAndId srvGetVersion;
SrvEraseStartupConfig srvEraseConfig;
SrvSendStartupConfig srvSendStartupConfig(128);

// Declaration of thread condition variable
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

// declaring mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

std::string str;

int main(int argc, char *argv[]) {
#ifndef _ONLY_MANUAL_CFG
	//ProgramConfig::readConfigFromFile("");
#endif

#ifdef _ONLY_MANUAL_CFG
	ProgramConfig::manualConfig();
#endif

	s = std::make_shared<Serial>();

	srvRunningConfig.setSerialContext(s);
	srvGetVersion.setSerialContext(s);
	srvEraseConfig.setSerialContext(s);
	srvSendStartupConfig.setSerialContext(s);

	srvGetVersion.setConditionVariable(std::shared_ptr<pthread_cond_t>(&cond1));
	srvRunningConfig.setConditionVariable(std::shared_ptr<pthread_cond_t>(&cond1));
	srvEraseConfig.setConditionVariable(std::shared_ptr<pthread_cond_t>(&cond1));

	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_RUNNING_CONFIG, &srvRunningConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_VERSION_AND_ID, &srvGetVersion));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_ERASE_STARTUP_CFG_RESP, &srvEraseConfig));
	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_PROGRAM_STARTUP_CFG_RESP, &srvSendStartupConfig));

	SerialWorker worker(s, callbackMap);

	std::vector<uint8_t> test;
	test.insert(test.begin(), 0x800, 0xAB);
	std::shared_ptr<std::vector<uint8_t>> pointerTxTest = std::shared_ptr<std::vector<uint8_t>>(&test);


	if (argc > 1) {
		s->init(argv[1], B9600);
	}
	else {
		s->init("/dev/ttyUSB1", B9600);
	}

	worker.start();
	worker.waitForStartup();
//	srvRunningConfig.sendRequest();
//	s->waitForTransmissionDone();
	srvGetVersion.sendRequest();
	s->waitForTransmissionDone();

	// wait for software version
    pthread_mutex_lock(&lock);
    pthread_cond_wait(&cond1, &lock);
    pthread_mutex_unlock(&lock);

	srvEraseConfig.sendRequest();

    pthread_mutex_lock(&lock);
    // wait for configuration to be received
    pthread_cond_wait(&cond1, &lock);
    pthread_mutex_unlock(&lock);

    std::cout << "erase done" << std::endl;

    srvSendStartupConfig.setDataForDownload(std::move(test));
    srvSendStartupConfig.sendRequest();

//    DecodeVer0 decode(srvRunningConfig.getConfigurationData());
//
//    decode.getDescritpion(str);
//    std::cout << str << std::endl;

//	s->transmitKissFrame(pointerTxTest);
//	s->receiveKissFrame(pointerRxTest);
	worker.terminate();

	return 0;

}


