
#include <iostream>
#include <vector>
#include <memory>
#include "ProgramConfig.h"
#include "serial/Serial.h"
#include "serial/SerialWorker.h"

#include "services/ServicesIds.h"
#include "services/SrvGetRunningConfig.h"

std::map<uint8_t, IService*> callbackMap;

std::shared_ptr<Serial> s = std::make_shared<Serial>();

SrvGetRunningConfig srvRunningConfig (s);

int main(int argc, char *argv[]) {
#ifndef _ONLY_MANUAL_CFG
	//ProgramConfig::readConfigFromFile("");
#endif

#ifdef _ONLY_MANUAL_CFG
	ProgramConfig::manualConfig();
#endif

	callbackMap.insert(std::pair<uint8_t, IService *>(KISS_RUNNING_CONFIG, &srvRunningConfig));

	SerialWorker worker(s, callbackMap);

	std::vector<uint8_t> test{0x20};
	std::shared_ptr<std::vector<uint8_t>> pointerTxTest = std::make_shared<std::vector<uint8_t>>(test);

	std::vector<uint8_t> testRx;
	std::shared_ptr<std::vector<uint8_t>> pointerRxTest = std::make_shared<std::vector<uint8_t>>(testRx);

	if (argc > 1) {
		s->init(argv[1], B9600);
	}
	else {
		s->init("/dev/ttyUSB0", B9600);
	}

	worker.start();
	s->transmitKissFrame(pointerTxTest);
//	s->receiveKissFrame(pointerRxTest);
	worker.terminate();

	return 0;

}


