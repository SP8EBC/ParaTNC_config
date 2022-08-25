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

SrvGetRunningConfig::SrvGetRunningConfig(std::shared_ptr<Serial>  serial) : s(serial), currentRegion(UNDEF) {

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

	CurrentConfigRegion receivedCurrentRegion = (CurrentConfigRegion)frame.at(2);

	// if currently no data transfer is in progress
	if (currentRegion == UNDEF) {

		// and a first frame of the transfer (a header) has been received
		if (receivedCurrentRegion == CURRENT_CONFIG_FIRST || receivedCurrentRegion == CURRENT_CONFIG_SECOND) {
			currentRegion = receivedCurrentRegion;

			// how many KISS packets is expected
			expectedKissFrames = frame.at(3);

			std::cout << "I = SrvGetRunningConfig::callback, transfer started, expectedKissFrames: " << (int)expectedKissFrames  << ", currentRegion: " << (int)currentRegion << std::endl;
		}
		else if (receivedCurrentRegion == CURRENT_CONFIG_CONTENT) {
			reset();	// if data frame has been received without prior header this should trigger an error
		}

	}
	else {
		// if transfer is initiated
		uint8_t remainingFrames = frame.at(3);
		std::cout << "I = SrvGetRunningConfig::callback, current frame seq id: " << (int)remainingFrames  << ", frame size: " << frame.size() << " (0x" << std::hex << frame.size() << std::dec << ")" << std::endl;

//		auto moveFrom = std::make_move_iterator(frame.begin() + 5);
//		auto moveTo = std::make_move_iterator(frame.end());
//
//		// move data into output buffer
//		configurationData.insert(configurationData.end(), moveFrom, moveTo);
		auto copyFrom = frame.begin() + 4;
		auto copyTo = frame.end();

		configurationData.insert(configurationData.end(), copyFrom, copyTo);
	}

}

void SrvGetRunningConfig::reset() {
}

SrvGetRunningConfig& SrvGetRunningConfig::operator=(SrvGetRunningConfig &&other) {
	return * this;
}

void SrvGetRunningConfig::sendRequest() {
	s->transmitKissFrame(SrvGetRunningConfig::requestData);

}
