/*
 * GetRunningConfig.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: mateusz
 */
#include "stdafx.h"

#include "SrvGetRunningConfig.h"

#include "../types/CurrentConfigRegion.h"
#include "../config/ValidateVer0.h"

#include "../shared/kiss_communication_service_ids.h"

#include <iostream>
#include <stdio.h>

const std::vector<uint8_t> SrvGetRunningConfig::requestData((size_t)1, KISS_GET_RUNNING_CONFIG);

SrvGetRunningConfig::SrvGetRunningConfig() : currentRegion(UNDEF), expectedKissFrames(0), validatedOk(false) {
#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	syncEvent = OpenEvent(EVENT_ALL_ACCESS, false, L"ServiceSyncEv");
#else
	conditionVariable = 0;
#endif
	lastReceivedKissFrameNumber = 0;
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
	size_t dataSize = frame->size() - 4;

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
		lastReceivedKissFrameNumber = currentFrame;
		std::cout << "I = SrvGetRunningConfig::callback, current frame seq id: " << (int)currentFrame  << ", frame size: " << frame->size() << " (0x" << std::hex << frame->size() << std::dec << ")" << std::endl;

		std::vector<uint8_t>::const_iterator copyFrom = frame->begin() + 4;
		std::vector<uint8_t>::const_iterator copyTo = frame->end();

		configurationData.insert(configurationData.end(), copyFrom, copyTo);

		if (currentFrame + 1 == expectedKissFrames) {

			// terminate reception
//			currentRegion = UNDEF;

			// verify CRC
			this->validatedOk = validate->checkValidate(configurationData);

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
			SetEvent(syncEvent);
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

void SrvGetRunningConfig::receiveSynchronously(IService_NegativeResponseCodeCbk cbk) {

	bool hasFirstFrame = false;

	if (s) {
		std::vector<uint8_t> response;

		// receive a response
		s->receiveKissFrame(response);

		if (response.size() > 1) {
			// get frame type, which was received
			uint8_t frameType = response[1];

			if (frameType == KISS_NEGATIVE_RESPONSE_SERVICE)
			{
				std::cout << "E = SrvGetRunningConfig::receiveSynchronously, NRC received: 0x" <<
					std::hex << response[2] << std::dec << std::endl;
			}
			else if (frameType == KISS_RUNNING_CONFIG)
			{
				// use callback manualy
				this->callback(&response);

				// check if first frame has been received correctly
				if (currentRegion == CURRENT_CONFIG_FIRST || currentRegion == CURRENT_CONFIG_SECOND) {
					hasFirstFrame = true;
				}
			}
			else
			{
				std::cout << "E = SrvGetRunningConfig::receiveSynchronously, response to unknown service!" << std::endl;
			}


		}

		// continue only if first frame has been received
		if (hasFirstFrame) {
			std::cout << "E = SrvGetRunningConfig::receiveSynchronously, expectedKissFrames: " << expectedKissFrames << std::endl;

			// receive all message in a loop. please be aware than a value of
			// 'lastReceivedKissFrameNumber' comes from frame content, so if
			// any frame will be skipped or missed for any reason this loop
			// will not detect that. it can iterate less times than amount
			// of frames it should receive.
			while (lastReceivedKissFrameNumber + 1 != expectedKissFrames) {

				// clear vector and prepare it to next frame from controler
				response.clear();

				// receive a response
				s->receiveKissFrame(response);

				if (response.size() > 1) {
					// get frame type, which was received
					uint8_t frameType = response[1];

					if (frameType == KISS_NEGATIVE_RESPONSE_SERVICE)
					{
						std::cout << "E = SrvGetRunningConfig::receiveSynchronously, NRC received: 0x" <<
							std::hex << response[2] << std::dec << std::endl;
					}
					else if (frameType == KISS_RUNNING_CONFIG)
					{
						// use callback manualy
						this->callback(&response);

						// check if first frame has been received correctly
						if (currentRegion == CURRENT_CONFIG_FIRST || currentRegion == CURRENT_CONFIG_SECOND) {
							hasFirstFrame = true;
						}
					}
					else
					{
						std::cout << "E = SrvGetRunningConfig::receiveSynchronously, response to unknown service!" << std::endl;
					}
				}
			}
		}
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
