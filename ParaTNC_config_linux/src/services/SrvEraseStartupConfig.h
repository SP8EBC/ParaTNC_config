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
#include "../types/ErasingProgrammingRes.h"

#include <memory>

class SrvEraseStartupConfig: public IService {

	/**
	 * Pointer to serial port context used to talk with TNC
	 */
	std::shared_ptr<Serial> s;

	/**
	 *
	 */
	erasing_programming_result_t operationResult;

	const static shared_ptr<std::vector<uint8_t>> requestData;

	const static std::string resultToString(erasing_programming_result_t res) {
		switch(res) {
		case RESULT_IDLE:		return "RESULT_IDLE";
		case RESULT_PENDING:	return "RESULT_PENDING";
		case RESULT_ERASED:		return "RESULT_ERASED";
		case RESULT_ERROR:		return "RESULT_ERROR";
		}

		return "";
	}

public:

	void sendRequest();

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
