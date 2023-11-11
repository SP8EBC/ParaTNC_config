/*
 * GetRunningConfig.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: mateusz
 */
#include "stdafx.h"

#include "SrvGetRunningConfig.h"

#include "../types/CurrentConfigRegion.h"
#include "../config/decode/ValidateVer0.h"

#include "../shared/kiss_communication_service_ids.h"

#include <iostream>
#include <stdio.h>

const std::vector<uint8_t> SrvGetRunningConfig::requestData((size_t)1, KISS_GET_RUNNING_CONFIG);

SrvGetRunningConfig::SrvGetRunningConfig() : currentRegion(UNDEF), expectedKissFrames(0) {
#if defined (_MSC_VER) && (_MSC_VER <= 1400)

#else
	conditionVariable = 0;
#endif
	s = 0;
	validate = new ValidateVer0();
}

SrvGetRunningConfig::~SrvGetRunningConfig() {
}

SrvGetRunningConfig& SrvGetRunningConfig::operator=(const SrvGetRunningConfig &other) {
	return * this;
}

void SrvGetRunningConfig::callback(const std::vector<uint8_t> * frame) {

	// data payload size - this is a size of data read from flash memory
	uint8_t dataSize = frame->size() - 4;

	CurrentConfigRegion receivedCurrentRegion = (CurrentConfigRegion)frame->at(2);

	// if currently no data transfer is in progress
	if (currentRegion == UNDEF) {

		// and a first frame of the transfer (a header) has been received
		if (receivedCurrentRegion == CURRENT_CONFIG_FIRST || receivedCurrentRegion == CURRENT_CONFIG_SECOND) {
			currentRegion = receivedCurrentRegion;

			// how many KISS packets is expected
			expectedKissFrames = frame->at(3);

			std::cout << "I = SrvGetRunningConfig::callback, transfer started, expectedKissFrames: " << (int)expectedKissFrames  << ", currentRegion: " << (int)currentRegion << std::endl;
		}
		else if (receivedCurrentRegion == CURRENT_CONFIG_CONTENT) {
			reset();	// if data frame has been received without prior header this should trigger an error
		}

	}
	else {
		// if transfer is initiated
		uint8_t currentFrame = frame->at(3);
		std::cout << "I = SrvGetRunningConfig::callback, current frame seq id: " << (int)currentFrame  << ", frame size: " << frame->size() << " (0x" << std::hex << frame->size() << std::dec << ")" << std::endl;

		std::vector<uint8_t>::const_iterator copyFrom = frame->begin() + 4;
		std::vector<uint8_t>::const_iterator copyTo = frame->end();

		configurationData.insert(configurationData.end(), copyFrom, copyTo);

		if (currentFrame + 1 == expectedKissFrames) {

			// terminate reception
//			currentRegion = UNDEF;

			// verify CRC
			validate->checkValidate(configurationData);

#if defined (_MSC_VER) && (_MSC_VER <= 1400)

#else
			if (conditionVariable) {
				pthread_cond_signal(conditionVariable);
			}
#endif
		}
		else if (currentFrame + 1 > expectedKissFrames) {
			std::cout << "E = SrvGetRunningConfig::callback, current frame seq id: " << (int)currentFrame  << ", frame size: " << frame->size() << " (0x" << std::hex << frame->size() << std::dec << ")" << std::endl;

		}
	}

}

void SrvGetRunningConfig::reset() {
}

void SrvGetRunningConfig::sendRequest() {
	if (s) {
		s->transmitKissFrame(SrvGetRunningConfig::requestData);
	}

}

bool SrvGetRunningConfig::storeToBinaryFile(std::string _in) {

	bool output = false;

	// open file for writing in binary mode
	FILE * f = fopen(_in.c_str(), "wb");

	// if file has been opened
	if (f != NULL) {
#if defined (_MSC_VER) && (_MSC_VER <= 1400)
		fwrite(&this->configurationData[0], this->configurationData.size(), 1, f);
#else
		fwrite(this->configurationData.data(), this->configurationData.size(), 1, f);
#endif
		fflush(f);

		fclose(f);

		output = true;
	}


	return output;
}
