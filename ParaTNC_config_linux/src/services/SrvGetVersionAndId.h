/*
 * SrvGetVersionAndId.h
 *
 *  Created on: Sep 4, 2022
 *      Author: mateusz
 */

#ifndef SRC_SERVICES_SRVGETVERSIONANDID_H_
#define SRC_SERVICES_SRVGETVERSIONANDID_H_

#include "IService.h"

#include "../serial/Serial.h"

class SrvGetVersionAndId: public IService {

	/**
	 * Pointer to serial port context used to talk with TNC
	 */
	std::shared_ptr<Serial> s;

	const static shared_ptr<std::vector<uint8_t>> requestData;

public:
	/**
	 * Sends a request to upload current running configuration
	 */
	void sendRequest();

	virtual ~SrvGetVersionAndId();
	SrvGetVersionAndId& operator=(SrvGetVersionAndId &&other);
	SrvGetVersionAndId& operator=(const SrvGetVersionAndId &other);
	SrvGetVersionAndId(SrvGetVersionAndId &&other);
	SrvGetVersionAndId(const SrvGetVersionAndId &other);
	SrvGetVersionAndId(std::shared_ptr<Serial>  serial);
	virtual void callback(
			const std::vector<unsigned char, std::allocator<unsigned char> > &frame)
					override;
};

#endif /* SRC_SERVICES_SRVGETVERSIONANDID_H_ */