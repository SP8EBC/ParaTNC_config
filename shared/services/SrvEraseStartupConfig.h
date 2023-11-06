/*
 * SrvEraseStartupConfig.h
 *
 *  Created on: Sep 8, 2022
 *      Author: mateusz
 */

#ifndef SRC_SERVICES_SRVERASESTARTUPCONFIG_H_
#define SRC_SERVICES_SRVERASESTARTUPCONFIG_H_

#include "IService.h"

#include "./serial/Serial.h"
#include "./types/ErasingProgrammingRes.h"
#include "./types/NRC.h"


#include <memory>

class SrvEraseStartupConfig: public IService {

	/**
	 * Pointer to serial port context used to talk with TNC
	 */
	Serial * s;

	/**
	 * Condition variable used to synchronize and lock another thread until
	 * memory erase is done. TNC sends ACK after flash operation is done.
	 */
	pthread_cond_t * conditionVariable;

	/**
	 *
	 */
	kiss_communication_nrc_t operationResult;

	const static std::vector<uint8_t> requestData;

public:

	virtual void sendRequest();

	SrvEraseStartupConfig();
	virtual ~SrvEraseStartupConfig();

	void setSerialContext(Serial *s) {
		this->s = s;
	}

	virtual void callback(
			const std::vector<unsigned char, std::allocator<unsigned char> > * frame);

	void setConditionVariable(pthread_cond_t * conditionVariable) {
		this->conditionVariable = conditionVariable;
	}
};

#endif /* SRC_SERVICES_SRVERASESTARTUPCONFIG_H_ */
