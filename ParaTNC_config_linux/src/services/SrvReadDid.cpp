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

	std::cout << "I = SrvReadDid::callback," << std::endl;


	if (conditionVariable != 0x00) {
		pthread_cond_signal(conditionVariable);
	}
}
