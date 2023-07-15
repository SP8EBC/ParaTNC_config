/*
 * SrvReadDid.cpp
 *
 *  Created on: Jul 6, 2023
 *      Author: mateusz
 */

#include "SrvReadDid.h"
#include "../shared/kiss_communication_service_ids.h"

#include <iostream>

SrvReadDid::SrvReadDid() {
	// TODO Auto-generated constructor stub

}

SrvReadDid::~SrvReadDid() {
	// TODO Auto-generated destructor stub
}

void SrvReadDid::sendRequest() {
	if (s != 0 && this->requestData.size() > 0) {
		s->transmitKissFrame(this->requestData);
	}
}

void SrvReadDid::sendRequestForDid(uint16_t did) {

	// check if all variables are set correctly
	if (this->s != NULL && this->conditionVariable != NULL) {
		// wipe content of request buffer
		requestData.clear();

		// service identifier
		requestData.push_back(KISS_READ_DID);

		// data identifier - LSB
		requestData.push_back(did & 0xFFu);

		// data identifier - MSB
		requestData.push_back((did & 0xFF00u) >> 8);

		sendRequest();
	}

}

void SrvReadDid::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > *frame) {

	std::vector<uint8_t>::const_iterator it = frame->begin();

	std::cout << "I = SrvReadDid::callback";

	for (int i = 0; i < frame->size(); i++) {
		std::cout << ", 0x" << std::hex << (int)frame->at(i);
	}

	std::cout << std::dec << std::endl;

	if (it != frame->end()) {
		it += 2;

		const uint8_t size_byte = *it;

		const bool is_string = ((size_byte & 0x80) == 0) ? true : false;

		if (is_string) {
			std::string content;

			do {
				content.push_back(*it);

				std::cout << "I = SrvReadDid::callback, string: " << content << std::endl;
			}while (++it != frame->end());

		}
		else {
			const uint8_t first_size = size_byte & 0x03;
			const uint8_t second_size = (size_byte & 0x0C) >> 2;
			const uint8_t third_size = (size_byte & 0x30) >> 4;

			std::cout << "I = SrvReadDid::callback, " <<
						"first_size: " 	<< 	(int)first_size 	<< ", " <<
						"second_size: " << 	(int)second_size 	<< ", " <<
						"third_size: " 	<< 	(int)third_size 	<< ", " << std::endl;

		}
	}


	if (conditionVariable != 0x00) {

		pthread_cond_signal(conditionVariable);
	}
}
