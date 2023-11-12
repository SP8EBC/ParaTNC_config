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
#include "../types/ErasingProgrammingRes.h"
#include "../types/NRC.h"

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <memory>

class SrvEraseStartupConfig: public IService {

	/**
	 * Pointer to serial port context used to talk with TNC
	 */
	Serial * s;

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	/**
	 * Event used to synchronize and lock another thread until
	 * memory erase is done. TNC sends ACK after flash operation is done.
	 */
	HANDLE syncEvent;
#else
	/**
	 * Condition variable used to synchronize and lock another thread until
	 * memory erase is done. TNC sends ACK after flash operation is done.
	 */
	pthread_cond_t * conditionVariable;
#endif

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

#if defined (_MSC_VER) && (_MSC_VER <= 1400)

#else
	void setConditionVariable(pthread_cond_t * conditionVariable) {
		this->conditionVariable = conditionVariable;
	}
#endif

};

#endif /* SRC_SERVICES_SRVERASESTARTUPCONFIG_H_ */
