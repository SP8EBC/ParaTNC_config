/*
 * GetRunningConfig.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: mateusz
 */

#include "SrvGetRunningConfig.h"
#include "../types/CurrentConfigRegion.h"

#include <iostream>

/**
 * GET_RUNNING_CONFIG has a constant request content so it can be embedded in this
 * a little bit complicated initialization
 */
const shared_ptr<std::vector<uint8_t>> SrvGetRunningConfig::requestData = std::make_shared<std::vector<uint8_t>>(std::vector<uint8_t>({0x20}));

SrvGetRunningConfig::SrvGetRunningConfig(std::shared_ptr<Serial>  serial) : s(serial) {

}

SrvGetRunningConfig::~SrvGetRunningConfig() {
}

SrvGetRunningConfig::SrvGetRunningConfig(const SrvGetRunningConfig &other) {

}

SrvGetRunningConfig::SrvGetRunningConfig(SrvGetRunningConfig &&other) {

}

SrvGetRunningConfig& SrvGetRunningConfig::operator=(const SrvGetRunningConfig &other) {
	return * this;
}

void SrvGetRunningConfig::callback(const std::vector<uint8_t> &frame) {

	// data payload size - this is a size of data read from flash memory
	uint8_t dataSize = frame.size() - 4;

	//uint8_t runningConfigRegion = frame.at(2);
	CurrentConfigRegion currentRegion = (CurrentConfigRegion)frame.at(2);

	std::cout << "I = SrvGetRunningConfig::callback, dataSize: " << dataSize  << std::endl;

}

SrvGetRunningConfig& SrvGetRunningConfig::operator=(SrvGetRunningConfig &&other) {
	return * this;
}

void SrvGetRunningConfig::sendRequest() {
	s->transmitKissFrame(SrvGetRunningConfig::requestData);

}
