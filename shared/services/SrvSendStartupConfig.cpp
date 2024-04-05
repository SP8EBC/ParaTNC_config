/*
 * SrvSendStartupConfig.cpp
 *
 *  Created on: Sep 8, 2022
 *      Author: mateusz
 */
#include "stdafx.h"

#include "SrvSendStartupConfig.h"

#include <iostream>

#include "../shared/kiss_communication_service_ids.h"

/**
 * How startup config is sent to the controller:
 *
 * - All CRC and programming counter handling is done by this application (but not this class)
 * - Configuration block is divided into 128 bytes blocks
 * - These block are send in consecutive order although it isn't entirely necessary to keep such order.
 *
 * 	 * The structure of input frame goes like that:
	 * FEND, KISS_PROGRAM_STARTUP_CFG, LN, OFFSET, data, data, (...), FEND
	 * LN is a size of data to be flashed, not the size of the frame itself
 */

SrvSendStartupConfig::SrvSendStartupConfig(int _singleFrameLn) : singleFrameLn(_singleFrameLn) {

	currentOffset = 0;


#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	syncEvent = OpenEvent(EVENT_ALL_ACCESS, false, L"ServiceSyncEv");

	internalSync = CreateEvent(NULL, false, false, L"SendConfigSyncEv");
#else
	internalSync = PTHREAD_COND_INITIALIZER;

	conditionVariable = 0;
#endif
	operationResult = NRC_POSITIVE;
	s = 0;

}

SrvSendStartupConfig::~SrvSendStartupConfig() {
}

SrvSendStartupConfig::SrvSendStartupConfig(const SrvSendStartupConfig &other) : singleFrameLn(other.singleFrameLn)
{
	currentOffset = 0;

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	syncEvent = OpenEvent(EVENT_ALL_ACCESS, false, L"ServiceSyncEv");

	internalSync = CreateEvent(NULL, false, false, L"SendConfigSyncEv");
#else
	internalSync = PTHREAD_COND_INITIALIZER;

	conditionVariable = 0;
#endif
	operationResult = NRC_POSITIVE;
	s = 0;
}



SrvSendStartupConfig& SrvSendStartupConfig::operator=(
		const SrvSendStartupConfig &other) {
	// TODO Auto-generated method stub

	return * this;
}

void SrvSendStartupConfig::sendRequest() {

#if defined (_MSC_VER) && (_MSC_VER <= 1400)

#else
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#endif

	size_t howManyFrames = dataForDownload.size() / singleFrameLn;

	std::vector<uint8_t>::iterator begin = dataForDownload.begin();

	// if a size of configuration block is not divisible by
	if ((dataForDownload.size() % singleFrameLn) != 0) {
		howManyFrames++;
	}

	// transmit all frames one after another
	for (size_t i = 0; i < howManyFrames; i++) {
		// reinitialize and preallocate vector with segmented data
		segmentedData = std::vector<uint8_t>();
		segmentedData.reserve(singleFrameLn + 4);
		std::vector<uint8_t>::iterator iterator = segmentedData.begin();

		segmentedData.insert(iterator, KISS_PROGRAM_STARTUP_CFG);
		segmentedData.insert(iterator + 1, singleFrameLn);
		segmentedData.insert(iterator + 2, this->currentOffset & 0xFF);
		segmentedData.insert(iterator + 3, (this->currentOffset & 0xFF00) >> 8);


		// check if this is last frame or not
		if (i == howManyFrames) {
			size_t lastDataLn = dataForDownload.size() - i * singleFrameLn;

			segmentedData.insert(segmentedData.end(), begin + currentOffset, begin + currentOffset + lastDataLn);
		}
		else {
			segmentedData.insert(segmentedData.end(), begin + currentOffset, begin + currentOffset + singleFrameLn);
		}

		std::cout << "I = SrvSendStartupConfig::sendRequest, currentOffset: 0x" << std::hex << (int)currentOffset  << std::dec << std::endl;

		// increase currrent offset
		currentOffset += singleFrameLn;

		// transmit fame to TNC
		if (s != 0) {
			s->transmitKissFrame(segmentedData);
		}

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
		// TODO: Do not ignore return value here
		WaitForSingleObject(internalSync, (DWORD)1234u);
#else
	    pthread_mutex_lock(&lock);

	    // wait for configuration to be received
	    pthread_cond_wait(&internalSync, &lock);

	    pthread_mutex_unlock(&lock);
#endif

	}

}

/**
 * This method is one exception from the rest of services. This shall be
 * used standalone, w/o call to sendRequest before.
 */
void SrvSendStartupConfig::receiveSynchronously(IService_NegativeResponseCodeCbk cbk) {
	if (s) {
		size_t howManyFrames = dataForDownload.size() / singleFrameLn;

		std::vector<uint8_t> response;

		std::vector<uint8_t>::iterator begin = dataForDownload.begin();

		// if a size of configuration block is not divisible by
		if ((dataForDownload.size() % singleFrameLn) != 0) {
			howManyFrames++;
		}

		// transmit all frames one after another
		for (size_t i = 0; i < howManyFrames; i++) {
			// reinitialize and preallocate vector with segmented data
			segmentedData = std::vector<uint8_t>();
			segmentedData.reserve(singleFrameLn + 4);
			std::vector<uint8_t>::iterator iterator = segmentedData.begin();

			segmentedData.insert(iterator, KISS_PROGRAM_STARTUP_CFG);
			segmentedData.insert(iterator + 1, singleFrameLn);
			segmentedData.insert(iterator + 2, this->currentOffset & 0xFF);
			segmentedData.insert(iterator + 3, (this->currentOffset & 0xFF00) >> 8);


			// check if this is last frame or not
			if (i == howManyFrames) {
				size_t lastDataLn = dataForDownload.size() - i * singleFrameLn;

				segmentedData.insert(segmentedData.end(), begin + currentOffset, begin + currentOffset + lastDataLn);
			}
			else {
				segmentedData.insert(segmentedData.end(), begin + currentOffset, begin + currentOffset + singleFrameLn);
			}

			std::cout << "I = SrvSendStartupConfig::receiveSynchronously, currentOffset: 0x" << std::hex << (int)currentOffset  << std::dec << std::endl;

			// increase currrent offset
			currentOffset += singleFrameLn;

			// transmit fame to TNC
			s->transmitKissFrame(segmentedData);

			// receive a response
			s->receiveKissFrame(response);

			if (response.size() > 1) {
				int32_t result = (int32_t)response.at(2);

				operationResult = (kiss_communication_nrc_t)result;

				std::cout << "I = SrvSendStartupConfig::receiveSynchronously, result: 0x" <<  std::hex << (int)result  << std::dec << " - " << nrcToString(operationResult) << std::endl;

			}

//	#if defined (_MSC_VER) && (_MSC_VER <= 1400)
//			// TODO: Do not ignore return value here
//			WaitForSingleObject(internalSync, (DWORD)1234u);
//	#else
//		    pthread_mutex_lock(&lock);
//
//		    // wait for configuration to be received
//		    pthread_cond_wait(&internalSync, &lock);
//
//		    pthread_mutex_unlock(&lock);
//	#endif

			response.clear();
		}
	}
}

void SrvSendStartupConfig::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > *frame) {

	int32_t result = (int32_t)frame->at(2);

	operationResult = (kiss_communication_nrc_t)result;

	std::cout << "I = SrvSendStartupConfig::callback, result: 0x" <<  std::hex << (int)result  << std::dec << " - " << nrcToString(operationResult) << std::endl;

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	SetEvent(internalSync);

	SetEvent(syncEvent);
#else
	pthread_cond_signal(&internalSync);

	if (conditionVariable) {
		pthread_cond_signal(conditionVariable);
	}
#endif
}
