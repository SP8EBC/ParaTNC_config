/*
 * SrvSendStartupConfig.cpp
 *
 *  Created on: Sep 8, 2022
 *      Author: mateusz
 */

#include "SrvSendStartupConfig.h"
#include "ServicesIds.h"

#include <iostream>

#include <pthread.h>

#include "../shared/kiss_communication_service_ids.h"

/**
 * How startup config is set to the controller:
 *
 * - All CRC and programming counter handling is done by this application (but not this class)
 * - Configuration block is divided into 128 bytes blocks
 * - These block are send in consecutive order although it isn't entirely necessary to keep such order.
 *
 * 	 * The structure of input frame goes like that:
	 * FEND, KISS_PROGRAM_STARTUP_CFG, LN, OFFSET, data, data, (...), FEND
	 * LN is a size of data to be flashed, not the size of the frame itself
 */

SrvSendStartupConfig::SrvSendStartupConfig(int singleFrameLn): currentOffset(0), singleFrameLn(singleFrameLn) {

	internalSync = PTHREAD_COND_INITIALIZER;

}

SrvSendStartupConfig::~SrvSendStartupConfig() {
	// TODO Auto-generated destructor stub
}

SrvSendStartupConfig::SrvSendStartupConfig(const SrvSendStartupConfig &other) : singleFrameLn(other.singleFrameLn),
																				currentOffset(0) {
	;
}

SrvSendStartupConfig::SrvSendStartupConfig(SrvSendStartupConfig &&other) : singleFrameLn(other.singleFrameLn),
																			currentOffset(0) {

}

SrvSendStartupConfig& SrvSendStartupConfig::operator=(
		const SrvSendStartupConfig &other) {
	// TODO Auto-generated method stub

}

SrvSendStartupConfig& SrvSendStartupConfig::operator=(
		SrvSendStartupConfig &&other) {
	// TODO Auto-generated method stub

}

void SrvSendStartupConfig::sendRequest() {

	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	int howManyFrames = dataForDownload.size() / singleFrameLn;

	auto begin = dataForDownload.begin();
	auto end = dataForDownload.end();

	// if a size of configuration block is not divisible by
	if ((dataForDownload.size() % singleFrameLn) != 0) {
		howManyFrames++;
	}

	// transmit all frames one after another
	for (int i = 0; i < howManyFrames; i++) {
		// reinitialize and preallocate vector with segmented data
		segmentedData = std::make_shared<std::vector<uint8_t>>();
		segmentedData->reserve(singleFrameLn + 4);
		auto iterator = segmentedData->begin();

		segmentedData->insert(iterator, KISS_PROGRAM_STARTUP_CFG);
		segmentedData->insert(iterator + 1, singleFrameLn);
		segmentedData->insert(iterator + 2, this->currentOffset & 0xFF);
		segmentedData->insert(iterator + 3, (this->currentOffset & 0xFF00) >> 8);


		// check if this is last frame or not
		if (i == howManyFrames) {
			int lastDataLn = dataForDownload.size() - i * singleFrameLn;

			segmentedData->insert(segmentedData->end(), begin + currentOffset, begin + currentOffset + lastDataLn);
		}
		else {
			segmentedData->insert(segmentedData->end(), begin + currentOffset, begin + currentOffset + singleFrameLn);
		}

		std::cout << "I = SrvSendStartupConfig::sendRequest, currentOffset: 0x" << std::hex << (int)currentOffset  << std::dec << std::endl;

		// increase currrent offset
		currentOffset += singleFrameLn;

		// transmit fame to TNC
		if (s) {
			s->transmitKissFrame(segmentedData);
		}

	    pthread_mutex_lock(&lock);

	    // wait for configuration to be received
	    pthread_cond_wait(&internalSync, &lock);

	    pthread_mutex_unlock(&lock);


	}

}

void SrvSendStartupConfig::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > &frame) {

	int32_t result = (int32_t)frame.at(2);

	operationResult = (erasing_programming_result_t)result;

	std::cout << "I = SrvSendStartupConfig::callback, result: 0x" <<  std::hex << (int)result  << std::dec << " - " << resultToString(operationResult) << std::endl;

	pthread_cond_signal(&internalSync);

	if (conditionVariable) {
		pthread_cond_signal(conditionVariable.get());
	}
}
