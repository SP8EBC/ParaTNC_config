/*
 * SrvEraseStartupConfig.cpp
 *
 *  Created on: Sep 8, 2022
 *      Author: mateusz
 */

#include "SrvEraseStartupConfig.h"
#include "../shared/kiss_communication_service_ids.h"

#include <vector>
#include <iostream>

//const shared_ptr<std::vector<uint8_t>> SrvEraseStartupConfig::requestData = std::make_shared<std::vector<uint8_t>>(std::vector<uint8_t>({0x22}));

std::vector<uint8_t> SrvEraseStartupConfig::requestData;

void SrvEraseStartupConfig::sendRequest() {
	if (s) {
		s->transmitKissFrame(SrvEraseStartupConfig::requestData);
	}
}

SrvEraseStartupConfig::SrvEraseStartupConfig() {
	SrvEraseStartupConfig::requestData->push_back(KISS_ERASE_STARTUP_CFG);
}

void SrvEraseStartupConfig::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > &frame) {

	int32_t result = (int32_t)frame.at(2);

	operationResult = (erasing_programming_result_t)result;

	std::cout << "I = SrvEraseStartupConfig::callback, result: 0x" <<  std::hex << (int)result  << std::dec << " - " << resultToString(operationResult) << std::endl;

	if (conditionVariable != 0x00) {
		pthread_cond_signal(conditionVariable);
	}

}
