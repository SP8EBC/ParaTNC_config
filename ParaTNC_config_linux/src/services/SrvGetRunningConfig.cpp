/*
 * GetRunningConfig.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: mateusz
 */

#include "SrvGetRunningConfig.h"

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
	std::cout << "I = SrvGetRunningConfig::callback " << std::endl;

}

SrvGetRunningConfig& SrvGetRunningConfig::operator=(SrvGetRunningConfig &&other) {
	return * this;
}

void SrvGetRunningConfig::sendRequest() {
	s->transmitKissFrame(SrvGetRunningConfig::requestData);

}
