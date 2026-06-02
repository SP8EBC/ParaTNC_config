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

	std::string boardType;

	std::string softwareVersion;

	char protocolVersion;

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
	 * @brief sends prepared request to the TNC. It is assumed that either a content of the
	 * request is already prepared, or it is fixed as per protocol definition
	 * @note in case of GetVersionAndIt request content is fixed and const
	 */
	virtual void sendRequest();

	/**
	 * @brief To be used after sending a request to a TNC, to lock calling thread while waiting
	 * for TNC and response, and automatically call a callback after this response is received
	 * @note callback is called from a context if this function
	 * @param cbk called in case of Negative Response Code is received 
	 */
	virtual void receiveSynchronously(IService_NegativeResponseCodeCbk cbk);

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

	std::string & getBoardType() {
		return this->boardType;
	}

	std::string & getSoftwareVersion() {
		return this->softwareVersion;
	}

	char getProtocolVersion() {
		return this->protocolVersion;
	}

#if defined (_MSC_VER) && (_MSC_VER <= 1400)

#else
	void setConditionVariable(pthread_cond_t * conditionVariable) {
		this->conditionVariable = conditionVariable;
	}
#endif

};

#endif /* SRC_SERVICES_SRVGETVERSIONANDID_H_ */
