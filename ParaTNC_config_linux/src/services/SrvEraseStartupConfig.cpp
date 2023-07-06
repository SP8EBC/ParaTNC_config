/*
 * SrvEraseStartupConfig.cpp
 *
 *  Created on: Sep 8, 2022
 *      Author: mateusz
 */

#include "SrvEraseStartupConfig.h"
#include "../types/NRC.h"
#include "../shared/kiss_communication_service_ids.h"

#include <vector>
#include <iostream>

const std::vector<uint8_t> SrvEraseStartupConfig::requestData((size_t)1, KISS_ERASE_STARTUP_CFG);

void SrvEraseStartupConfig::sendRequest() {
	if (s != 0) {
		s->transmitKissFrame(SrvEraseStartupConfig::requestData);
	}
}

SrvEraseStartupConfig::SrvEraseStartupConfig() {
	operationResult = NRC_POSITIVE;
	s = 0;
	conditionVariable = 0;
}

SrvEraseStartupConfig::~SrvEraseStartupConfig() {
}

void SrvEraseStartupConfig::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> >  * frame) {

	int32_t result = (int32_t)frame->at(2);

	operationResult = (kiss_communication_nrc_t)result;

	std::cout << "I = SrvEraseStartupConfig::callback, result: 0x" <<  std::hex << (int)result  << std::dec << " - " << nrcToString(operationResult) << std::endl;

	if (conditionVariable != 0x00) {
		pthread_cond_signal(conditionVariable);
	}

}
