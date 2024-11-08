/*
 * SrvReadMemory.cpp
 *
 *  Created on: Aug 3, 2024
 *      Author: mateusz
 */

#include "SrvReadMemory.h"
#include "../shared/kiss_communication_service_ids.h"


#include <iostream>

SrvReadMemory::SrvReadMemory() {
#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	syncEvent = OpenEvent(EVENT_ALL_ACCESS, false, L"ServiceSyncEv");
#else
	conditionVariable = 0;
	s = 0;
#endif
}

void SrvReadMemory::sendRequest() {
	if (s != 0 && this->requestData.size() > 0) {
		s->transmitKissFrame(this->requestData);
	}
}

void SrvReadMemory::sendRequestForMemoryRange(uint32_t address, uint8_t size) {

	// check if all variables are set correctly
	if (this->s != NULL) {
		// wipe content of request buffer
		requestData.clear();

		// service identifier
		requestData.push_back(KISS_READ_MEM_ADDR);

		// 'size_format' - currently only one byte of size and four byte of address
		requestData.push_back(0x14u);

		// address
		requestData.push_back((uint8_t)((address & 0xFF000000u) >> 24));
		requestData.push_back((uint8_t)((address & 0x00FF0000u) >> 16));
		requestData.push_back((uint8_t)((address & 0x0000FF00u) >> 8));
		requestData.push_back((uint8_t)((address & 0x000000FFu)) );

		// size in bytes
		requestData.push_back(size);


		sendRequest();
	}
}

void SrvReadMemory::receiveSynchronously() {
	if (s) {
		std::vector<uint8_t> response;

		// receive a response
		s->receiveKissFrame(response);

		if (response.size() > 1) {
			// get frame type, which was received
			uint8_t frameType = response[1];

			if (frameType == KISS_NEGATIVE_RESPONSE_SERVICE)
			{
				std::cout << "E = SrvReadMemory::receiveSynchronously, NRC received: 0x" <<
					std::hex << response[2] << std::dec << std::endl;
			}
			else if (frameType == KISS_READ_MEM_ADDR_RESP)
			{
				// use callback manualy
				this->callback(&response);
			}
			else
			{
				std::cout << "E = SrvReadMemory::receiveSynchronously, response to unknown service!" << std::endl;
			}


		}
	}
}

SrvReadMemory::~SrvReadMemory() {
	// TODO Auto-generated destructor stub
}

void SrvReadMemory::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > *frame) {

	std::vector<uint8_t>::const_iterator it = frame->begin();

	if (it != frame->end()) {
		// get response length - subtract FEND, NONSTANDARD, response_size + 5, KISS_READ_MEM_ADDR_RESP, (...), FEND
		// this gives amount of bytes read from the memory itself.
		const uint8_t response_size = (uint8_t)(*it) - 5;

		// skip service id
		it++;

		this->responseData.clear();

		// preallocate space for maximum data lenght the controller can return at once
		responseData.reserve(255u);

		for (int i = 0; i < response_size; i++ ) {
			responseData.push_back(*it);
			it++;
		}

	}

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	SetEvent(syncEvent);
#else
	if (conditionVariable != 0x00) {

		pthread_cond_signal(conditionVariable);
	}
#endif
}
