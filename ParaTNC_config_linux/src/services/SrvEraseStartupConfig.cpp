/*
 * SrvEraseStartupConfig.cpp
 *
 *  Created on: Sep 8, 2022
 *      Author: mateusz
 */

#include "SrvEraseStartupConfig.h"

#include <vector>

const shared_ptr<std::vector<uint8_t>> SrvEraseStartupConfig::requestData = std::make_shared<std::vector<uint8_t>>(std::vector<uint8_t>({0x22}));

SrvEraseStartupConfig::SrvEraseStartupConfig() {
	// TODO Auto-generated constructor stub

}

SrvEraseStartupConfig::~SrvEraseStartupConfig() {
	// TODO Auto-generated destructor stub
}

SrvEraseStartupConfig::SrvEraseStartupConfig(
		const SrvEraseStartupConfig &other) {
	// TODO Auto-generated constructor stub

}

SrvEraseStartupConfig::SrvEraseStartupConfig(SrvEraseStartupConfig &&other) {
	// TODO Auto-generated constructor stub

}

SrvEraseStartupConfig& SrvEraseStartupConfig::operator=(
		const SrvEraseStartupConfig &other) {
	//return other;

}

SrvEraseStartupConfig& SrvEraseStartupConfig::operator=(
		SrvEraseStartupConfig &&other) {
	//return * other;

}

void SrvEraseStartupConfig::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > &frame) {
}
