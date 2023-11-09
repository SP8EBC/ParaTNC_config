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

class SrvGetVersionAndId: public IService {

	/**
	 * Pointer to serial port context used to talk with TNC
	 */
	Serial * s;

	/**
	 * Condition variable used to synchronize and lock another thread until
	 * memory erase is done. TNC sends ACK after flash operation is done.
	 */
	pthread_cond_t * conditionVariable;

	const static std::vector<uint8_t> requestData;

	static bool hyphenCheck(char x);
public:
	/**
	 * Sends a request to upload current running configuration
	 */
	virtual void sendRequest();

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

	void setConditionVariable(pthread_cond_t * conditionVariable) {
		this->conditionVariable = conditionVariable;
	}
};

#endif /* SRC_SERVICES_SRVGETVERSIONANDID_H_ */
