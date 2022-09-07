/*
 * SrvGetVersionAndId.cpp
 *
 *  Created on: Sep 4, 2022
 *      Author: mateusz
 */

#include "SrvGetVersionAndId.h"
#include "ServicesIds.h"

#include <iostream>
#include <algorithm>

const shared_ptr<std::vector<uint8_t>> SrvGetVersionAndId::requestData = std::make_shared<std::vector<uint8_t>>(std::vector<uint8_t>({KISS_GET_VERSION_AND_ID}));

SrvGetVersionAndId::~SrvGetVersionAndId() {
}

SrvGetVersionAndId& SrvGetVersionAndId::operator=(SrvGetVersionAndId &&other) {
	return * this;
}

SrvGetVersionAndId& SrvGetVersionAndId::operator=(
		const SrvGetVersionAndId &other) {
	return * this;
}

SrvGetVersionAndId::SrvGetVersionAndId(SrvGetVersionAndId &&other) {

}

SrvGetVersionAndId::SrvGetVersionAndId(const SrvGetVersionAndId &other) {

}

SrvGetVersionAndId::SrvGetVersionAndId() {

}

void SrvGetVersionAndId::sendRequest() {
	if (s) {
		s->transmitKissFrame(SrvGetVersionAndId::requestData);
	}

}

void SrvGetVersionAndId::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > &frame) {

	int i = 0;

	// string to store full ID
	std::string id;

	// array to store
	std::array<std::string, 3> splited;

	// extract ID string
	std::for_each(frame.begin() + 2, frame.end(), [&id](auto x)  {id.append(1, x);});

	auto startIt = id.begin();
	auto foundIt = id.begin();

	// split string per '-'
	do {
		foundIt = std::find_if(startIt, id.end(), [](char x) { if (x == '-') return true; else return false;});

		splited[i] = std::string(startIt, foundIt);

		startIt = foundIt + 1;
	} while (++i < 3);


	std::cout << "I = SrvGetVersionAndId::callback, splited[0]: " << splited[0] << ", splited[1]: " << splited[1] << ", splited[2]: " << splited[2] <<  std::endl;


}
