/*
 * SrvSendStartupConfig.cpp
 *
 *  Created on: Sep 8, 2022
 *      Author: mateusz
 */

#include "SrvSendStartupConfig.h"

SrvSendStartupConfig::SrvSendStartupConfig(std::vector<uint8_t> & configData) {


}

SrvSendStartupConfig::~SrvSendStartupConfig() {
	// TODO Auto-generated destructor stub
}

SrvSendStartupConfig::SrvSendStartupConfig(const SrvSendStartupConfig &other) {
	// TODO Auto-generated constructor stub

}

SrvSendStartupConfig::SrvSendStartupConfig(SrvSendStartupConfig &&other) {
	// TODO Auto-generated constructor stub

}

SrvSendStartupConfig& SrvSendStartupConfig::operator=(
		const SrvSendStartupConfig &other) {
	// TODO Auto-generated method stub

}

SrvSendStartupConfig& SrvSendStartupConfig::operator=(
		SrvSendStartupConfig &&other) {
	// TODO Auto-generated method stub

}

void SrvSendStartupConfig::callback(
		const std::vector<unsigned char, std::allocator<unsigned char> > &frame) {
}
