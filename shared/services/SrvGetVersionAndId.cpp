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
	conditionVariable = 0;
	s = 0;
}

void SrvGetVersionAndId::sendRequest() {
	if (s) {
		s->transmitKissFrame(SrvGetVersionAndId::requestData);
	}

}

bool SrvGetVersionAndId::hyphenCheck(char x) {
	 if (x == '-') return true; else return false;
}

void SrvGetVersionAndId::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > * frame) {

	int i = 0;

	// string to store full ID
	std::string id;

	// array to store
	std::string splited[3];

	// extract ID string
	for (size_t i = 0; i < frame->size(); i++) {
		id.append(1, frame->at(i));
	}

	std::string::iterator startIt = id.begin();
	std::string::iterator foundIt = id.begin();

	// split string per '-'
	do {
		foundIt = std::find_if(startIt, id.end(), SrvGetVersionAndId::hyphenCheck);

		splited[i] = std::string(startIt, foundIt);

		startIt = foundIt + 1;
	} while (++i < 3);


	std::cout << "I = SrvGetVersionAndId::callback, splited[0]: " << splited[0] << ", splited[1]: " << splited[1] << ", splited[2]: " << splited[2] <<  std::endl;

	if (conditionVariable != 0) {
		pthread_cond_signal(conditionVariable);
	}

}
