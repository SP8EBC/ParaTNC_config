/*
 * SrvEraseStartupConfig.h
 *
 *  Created on: Sep 8, 2022
 *      Author: mateusz
 */

#ifndef SRC_SERVICES_SRVERASESTARTUPCONFIG_H_
#define SRC_SERVICES_SRVERASESTARTUPCONFIG_H_

#include "IService.h"

#include "../serial/Serial.h"

#include <memory>

class SrvEraseStartupConfig: public IService {

	/**
	 * Pointer to serial port context used to talk with TNC
	 */
	std::shared_ptr<Serial> s;

	const static shared_ptr<std::vector<uint8_t>> requestData;

public:

	SrvEraseStartupConfig();
	virtual ~SrvEraseStartupConfig();
	SrvEraseStartupConfig(const SrvEraseStartupConfig &other);
	SrvEraseStartupConfig(SrvEraseStartupConfig &&other);
	SrvEraseStartupConfig& operator=(const SrvEraseStartupConfig &other);
	SrvEraseStartupConfig& operator=(SrvEraseStartupConfig &&other);

	void setSerialContext(const std::shared_ptr<Serial> &s) {
		this->s = s;
	}

	virtual void callback(
			const std::vector<unsigned char, std::allocator<unsigned char> > &frame);
};

#endif /* SRC_SERVICES_SRVERASESTARTUPCONFIG_H_ */
