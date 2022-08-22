/*
 * GetRunningConfig.h
 *
 *  Created on: Aug 21, 2022
 *      Author: mateusz
 */

#ifndef SRC_SERVICES_SRVGETRUNNINGCONFIG_H_
#define SRC_SERVICES_SRVGETRUNNINGCONFIG_H_

#include <memory>

#include "../serial/Serial.h"

#include "IService.h"

static enum GetRunningConfigFrameContent {
	FIRST_REGION	= 0x01,
	SECOND_REGION	= 0x02,
	DATA			= 0xFF
}GetRunningConfigFrameContent;

class SrvGetRunningConfig : public IService {

	std::shared_ptr<Serial> s;

	const static shared_ptr<std::vector<uint8_t>> requestData;
public:
	/**
	 * Sends a request to upload current running configuration
	 */
	void sendRequest();

	/**
	 * Callback to be used when a frame of type
	 */
	virtual void callback(const std::vector<uint8_t> & frame);

	SrvGetRunningConfig(std::shared_ptr<Serial>  serial);
	virtual ~SrvGetRunningConfig();
	SrvGetRunningConfig(const SrvGetRunningConfig &other);
	SrvGetRunningConfig(SrvGetRunningConfig &&other);
	SrvGetRunningConfig& operator=(const SrvGetRunningConfig &other);
	SrvGetRunningConfig& operator=(SrvGetRunningConfig &&other);
};

#endif /* SRC_SERVICES_SRVGETRUNNINGCONFIG_H_ */
