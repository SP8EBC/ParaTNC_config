/*
 * SrvReadDid.h
 *
 *  Created on: Jul 6, 2023
 *      Author: mateusz
 */

#ifndef SRC_SERVICES_SRVREADDID_H_
#define SRC_SERVICES_SRVREADDID_H_

#include "IService.h"

#include "./serial/Serial.h"

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
#include <windows.h>
#else
#include <pthread.h>
#endif

class SrvReadDid: public IService {

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

	virtual void receiveSynchronously();

	/**
	 * Sends request to read given DID
	 * @param did data identifier to be written
	 */
	void sendRequestForDid(uint16_t did);

	/**
	 * Returns
	 * @return
	 */
	std::vector<uint8_t> getRawResponse();

	SrvReadDid();
	virtual ~SrvReadDid();

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


};

#endif /* SRC_SERVICES_SRVREADDID_H_ */
