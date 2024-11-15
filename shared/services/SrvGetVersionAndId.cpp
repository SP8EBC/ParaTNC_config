/*
 * SrvGetVersionAndId.cpp
 *
 *  Created on: Sep 4, 2022
 *      Author: mateusz
 */
#include "stdafx.h"

#include "SrvGetVersionAndId.h"

#include <iostream>
#include <algorithm>

#include "../shared/kiss_communication_service_ids.h"
#include "../shared/exceptions/TimeoutE.h"

/**
 * Request
 */
const std::vector<uint8_t> SrvGetVersionAndId::requestData((size_t)1, KISS_GET_VERSION_AND_ID);


SrvGetVersionAndId::~SrvGetVersionAndId() {
}

SrvGetVersionAndId& SrvGetVersionAndId::operator=(const SrvGetVersionAndId &other) {
	return * this;
}

SrvGetVersionAndId::SrvGetVersionAndId() {
#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	syncEvent = OpenEvent(EVENT_ALL_ACCESS, false, L"ServiceSyncEv");
#else
	conditionVariable = 0;
#endif
	s = 0;
}

void SrvGetVersionAndId::sendRequest() {
	if (s) {
		s->transmitKissFrame(SrvGetVersionAndId::requestData);
	}

}

void SrvGetVersionAndId::receiveSynchronously(IService_NegativeResponseCodeCbk cbk) {
	if (s) {
		std::vector<uint8_t> response;

		// receive a response
		s->receiveKissFrame(response);

		if (response.size() > 1) {
			// get frame type, which was received
			uint8_t frameType = response[1];

			if (frameType == KISS_NEGATIVE_RESPONSE_SERVICE) 
			{
				std::cout << "E = SrvGetVersionAndId::receiveSynchronously, NRC received: 0x" << 
					std::hex << response[2] << std::dec << std::endl;

				if (cbk != NULL) 
				{
					cbk(response[2]);
				}
			}
			else if (frameType == KISS_VERSION_AND_ID) 
			{
				// use callback manualy
				this->callback(&response);
			}
			else
			{
				std::cout << "E = SrvGetVersionAndId::receiveSynchronously, response to unknown service!" << std::endl;
			}


		}


	}
}

bool SrvGetVersionAndId::hyphenCheck(char x) {
	 if (x == '-') return true; else return false;
}

SrvGetVersionAndId::SrvGetVersionAndId(const SrvGetVersionAndId &other) {
}

void SrvGetVersionAndId::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > * frame) {

	int i = 0;

	// string to store full ID
	std::string id;

	// array to store
	std::string splited[3];

	// extract ID string
	for (size_t it = 0; it < frame->size(); it++) {
		id.append(1, frame->at(it));
	}

	std::string::iterator startIt = id.begin();
	std::string::iterator foundIt = id.begin();

	// split string per '-'
	do {
		foundIt = std::find_if(startIt, id.end(), SrvGetVersionAndId::hyphenCheck);

		const std::string extracted = std::string(startIt, foundIt);

		splited[i] = extracted;

		// in case of last element in a version string find_if
		// will return pass-the-end iterator, as there is 
		// no another '-' in the string.
		if (foundIt != id.end()) {
			startIt = foundIt + 1;
		}
	} while (++i < 3);

	// 
	boardType = std::string(splited[0], 2, splited[0].length());
	softwareVersion = splited[1];
	protocolVersion = splited[2][0];

	std::cout << "I = SrvGetVersionAndId::callback, boardType: " << boardType << ", softwareVersion: " << softwareVersion << ", protocolVersion: " << protocolVersion <<  std::endl;

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	SetEvent(syncEvent);
#else
	if (conditionVariable != 0) {
		pthread_cond_signal(conditionVariable);
	}
#endif

}
