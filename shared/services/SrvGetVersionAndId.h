/*
 * SrvGetVersionAndId.h
 *
 *  Created on: Sep 4, 2022
 *      Author: mateusz
 */

#ifndef SRC_SERVICES_SRVGETVERSIONANDID_H_
#define SRC_SERVICES_SRVGETVERSIONANDID_H_

#include "IService.h"

#include "serial/Serial.h"

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
#include <windows.h>
#else
#include <pthread.h>
#endif

class SrvGetVersionAndId: public IService {

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

	const static std::vector<uint8_t> requestData;

	static bool hyphenCheck(char x);
public:
	/**
	 * Sends a request to upload current running configuration
	 */
	virtual void sendRequest();

	virtual void receiveSynchronously();

	virtual ~SrvGetVersionAndId();
	SrvGetVersionAndId& operator=(const SrvGetVersionAndId &other);
	SrvGetVersionAndId(const SrvGetVersionAndId &other);
	SrvGetVersionAndId();

	/**
	 * Callback used when frame with version has been received from TNC
	 * @param frame
	 */
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

#endif /* SRC_SERVICES_SRVGETVERSIONANDID_H_ */
