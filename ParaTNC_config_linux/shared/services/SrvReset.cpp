/*
 * SrvReset.cpp
 *
 *  Created on: Dec 2, 2024
 *      Author: mateusz
 */

#include "SrvReset.h"
#include <iostream>
/**
 * Request
 */
const std::vector<uint8_t> SrvReset::requestData = {KISS_RESTART, 0x03};

SrvReset::SrvReset() {
	// TODO Auto-generated constructor stub
	conditionVariable = NULL;
	s = 0;
}

void SrvReset::sendRequest() {
	if (s) {
		std::cout << "I = SrvReset::sendRequest" << std::endl;

		s->transmitKissFrame(SrvReset::requestData);
	}
}

void SrvReset::restart() {
	this->sendRequest();
}

void SrvReset::waitForTransmissionDone() {
	if (s) {
		s->waitForTransmissionDone();
	}
}

void SrvReset::receiveSynchronously(IService_NegativeResponseCodeCbk cbk) {
}

SrvReset::~SrvReset() {
	// TODO Auto-generated destructor stub
}

void SrvReset::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > *frame) {

	std::cout << "E = SrvReset::callback, frame->size(): " << frame->size() << std::endl;

}
