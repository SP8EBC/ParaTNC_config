/*
 * SrvSendStartupConfig.cpp
 *
 *  Created on: Sep 8, 2022
 *      Author: mateusz
 */

#include "SrvSendStartupConfig.h"
#include "ServicesIds.h"

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

SrvSendStartupConfig::SrvSendStartupConfig(std::vector<uint8_t> && configData, int singleFrameLn): currentOffset(0), singleFrameLn(singleFrameLn) {

	dataForDownload = std::move(configData);


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
		segmentedData = std::make_shared<std::vector<uint8_t>>(singleFrameLn + 3);

		segmentedData[0] = KISS_PROGRAM_STARTUP_CFG_RESP;
		segmentedData[1] = singleFrameLn;
		segmentedData[2] = this->currentOffset;

		// check if this is last frame or not
		if (i == howManyFrames) {
			int lastDataLn = dataForDownload.size() - i * singleFrameLn;

			segmentedData->insert(segmentedData->end(), begin + currentOffset, begin + currentOffset + lastDataLn);
		}
		else {
			segmentedData->insert(segmentedData->end(), begin + currentOffset, begin + currentOffset + singleFrameLn);
		}

		// increase currrent offset
		currentOffset += singleFrameLn;

		// transmit fame to TNC
		if (s) {
			s->transmitKissFrame(segmentedData);
		}


	}

}

void SrvSendStartupConfig::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > &frame) {
}
