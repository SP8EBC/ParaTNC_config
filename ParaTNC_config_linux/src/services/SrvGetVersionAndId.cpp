/*
 * SrvGetVersionAndId.cpp
 *
 *  Created on: Sep 4, 2022
 *      Author: mateusz
 */

#include "SrvGetVersionAndId.h"
#include "ServicesIds.h"

#include <iostream>

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

SrvGetVersionAndId::SrvGetVersionAndId(std::shared_ptr<Serial>  serial) : s(serial) {

}

void SrvGetVersionAndId::sendRequest() {
	s->transmitKissFrame(SrvGetVersionAndId::requestData);

}

void SrvGetVersionAndId::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > &frame) {

	std::cout << "I = SrvGetVersionAndId::callback " << std::endl;


}
