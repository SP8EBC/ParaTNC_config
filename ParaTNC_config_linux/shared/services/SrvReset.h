/*
 * SrvReset.h
 *
 *  Created on: Dec 2, 2024
 *      Author: mateusz
 */

#ifndef SHARED_SERVICES_SRVRESET_H_
#define SHARED_SERVICES_SRVRESET_H_

#include "IService.h"

#include "serial/Serial.h"

#include "../shared/kiss_communication_service_ids.h"
#include "../shared/exceptions/TimeoutE.h"

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
#include <windows.h>
#else
#include <pthread.h>
#endif

class SrvReset: public IService {
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
	const static std::vector<uint8_t> requestData;

	/**
	 * Data received from controller
	 */
	std::vector<uint8_t> responseData;


public:
	/**
	 * @brief sends prepared request to the TNC. It is assumed that either a content of the
	 * request is already prepared, or it is fixed as per protocol definition
	 */
	virtual void sendRequest();

	void restart();

	/**
	 * @brief locks calling thread until transmission from host PC to TNC is done
	 */
	void waitForTransmissionDone();

	/**
	 * @brief To be used after sending a request to a TNC, to lock calling thread while waiting
	 * for TNC and response, and automatically call a callback after this response is received
	 * @note callback is called from a context if this function
	 * @param cbk called in case of Negative Response Code is received 
	 */
	virtual void receiveSynchronously(IService_NegativeResponseCodeCbk cbk);

	SrvReset();
	virtual ~SrvReset();

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

#endif /* SHARED_SERVICES_SRVRESET_H_ */
