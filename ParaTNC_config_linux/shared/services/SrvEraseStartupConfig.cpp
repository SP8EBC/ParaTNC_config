/*
 * SrvEraseStartupConfig.cpp
 *
 *  Created on: Sep 8, 2022
 *      Author: mateusz
 */
#include "stdafx.h"

#include "SrvEraseStartupConfig.h"
//#include "../types/NRC.h"
#include "../kiss_communication_service_ids.h"

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
#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	syncEvent = OpenEvent(EVENT_ALL_ACCESS, false, L"ServiceSyncEv");
#else
	conditionVariable = 0;
#endif
	}

SrvEraseStartupConfig::~SrvEraseStartupConfig() {
}

void SrvEraseStartupConfig::receiveSynchronously(IService_NegativeResponseCodeCbk cbk) {
	if (s) {
		std::vector<uint8_t> response;

		// receive a response
		s->receiveKissFrame(response);

		if (response.size() > 1) {
			// get frame type, which was received
			uint8_t frameType = response[1];

			if (frameType == KISS_NEGATIVE_RESPONSE_SERVICE)
			{
				std::cout << "E = SrvEraseStartupConfig::receiveSynchronously, NRC received: 0x" <<
					std::hex << response[2] << std::dec << std::endl;
			}
			else if (frameType == KISS_ERASE_STARTUP_CFG_RESP)
			{
				// use callback manualy
				this->callback(&response);
			}
			else
			{
				std::cout << "E = SrvEraseStartupConfig::receiveSynchronously, response to unknown service!" << std::endl;
			}
		}
	}
}

void SrvEraseStartupConfig::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> >  * frame) {

	int32_t result = (int32_t)frame->at(2);

	operationResult = (kiss_communication_nrc_t)result;

	std::cout << "I = SrvEraseStartupConfig::callback, result: 0x" <<  std::hex << (int)result  << std::dec << " - " << operationResult << std::endl;

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	SetEvent(syncEvent);
#else
	if (conditionVariable != 0x00) {
		pthread_cond_signal(conditionVariable);
	}
#endif

}

void SrvEraseStartupConfig::nrcCallback(
		const enum kiss_communication_nrc_t nrc, bool isFromBackgroundAsyncThread) {
}
