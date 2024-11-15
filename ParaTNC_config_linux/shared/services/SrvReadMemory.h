/*
 * SrvReadMemory.h
 *
 *  Created on: Aug 3, 2024
 *      Author: mateusz
 */

#ifndef SHARED_SERVICES_SRVREADMEMORY_H_
#define SHARED_SERVICES_SRVREADMEMORY_H_

#include "IService.h"

#include "./serial/Serial.h"
#include "../types/DidResponse.h"

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
#include <windows.h>
#else
#include <pthread.h>
#endif


class SrvReadMemory: public IService {
	/**
	 * Pointer to serial port context used to talk with TNC
	 */
	Serial * s;

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	HANDLE syncEvent;
#else
	/**
	 * Condition variable used to synchronize and lock another thread until
	 * memory erase is done. TNC sends ACK after flash operation is done.
	 */
	pthread_cond_t * conditionVariable;
#endif

	/**
	 * Request data send to the controller, it is not const nor static as
	 * it contains DID number
	 */
	std::vector<uint8_t> requestData;

	/**
	 * Data received from controller
	 */
	std::vector<uint8_t> responseData;

public:
	/**
	 *
	 */
	virtual void sendRequest();

	void sendRequestForMemoryRange(uint32_t address, uint8_t size);

	void waitForTransmissionDone();

	virtual void receiveSynchronously(IService_NegativeResponseCodeCbk cbk);

	SrvReadMemory();
	virtual ~SrvReadMemory();

	virtual void callback(const std::vector<unsigned char, std::allocator<unsigned char> > * frame);

	void setSerialContext(Serial * s) {
		this->s = s;
	}

#if defined (_MSC_VER) && (_MSC_VER <= 1400)

#else
	void setConditionVariable(pthread_cond_t * conditionVariable) {
		this->conditionVariable = conditionVariable;
	}
#endif

	std::vector<uint8_t> getResponseData() const {
		return responseData;
	}

};

#endif /* SHARED_SERVICES_SRVREADMEMORY_H_ */
