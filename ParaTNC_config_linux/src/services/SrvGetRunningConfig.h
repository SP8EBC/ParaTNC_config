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
#include "../types/CurrentConfigRegion.h"

#include "../config/decode/IConfigDecode.h"
#include "../config/decode/IConfigValidate.h"

#include "IService.h"

class SrvGetRunningConfig : public IService {

	/**
	 * Pointer to serial port context used to talk with TNC
	 */
	std::shared_ptr<Serial> s;

	/**
	 *
	 */
	std::shared_ptr<pthread_cond_t> conditionVariable;

	/**
	 * This is a content of 'GET_RUNNING_CONFIG' request. It is so damn awkward definition,
	 * because functions for KISS frame transmission have only one overload. his overload expects
	 * a pointer to vector
	 */
	const static shared_ptr<std::vector<uint8_t>> requestData;

	/**
	 * Current memory area which is received from the TNC + and information if any reception is
	 * initiated. This holds an internal state
	 */
	CurrentConfigRegion currentRegion;

	/**
	 * How many frames is expected to be received from TNC
	 */
	uint16_t expectedKissFrames;

	/*
	 * Configuration data extacted from frames
	 */
	std::vector<uint8_t> configurationData;

	std::shared_ptr<IConfigValidate> validate;

	void reset();
public:
	/**
	 * Sends a request to upload current running configuration
	 */
	void sendRequest();

	/**
	 * Callback to be used when a frame of type
	 */
	virtual void callback(const std::vector<uint8_t> & frame);


	SrvGetRunningConfig();
	virtual ~SrvGetRunningConfig();
	SrvGetRunningConfig(const SrvGetRunningConfig &other);
	SrvGetRunningConfig(SrvGetRunningConfig &&other);
	SrvGetRunningConfig& operator=(const SrvGetRunningConfig &other);
	SrvGetRunningConfig& operator=(SrvGetRunningConfig &&other);

	void setSerialContext(const std::shared_ptr<Serial> &s) {
		this->s = s;
	}

	void setConditionVariable(
			const std::shared_ptr<pthread_cond_t> &conditionVariable) {
		this->conditionVariable = conditionVariable;
	}

	const std::vector<uint8_t>& getConfigurationData() const {
		return configurationData;
	}
};

#endif /* SRC_SERVICES_SRVGETRUNNINGCONFIG_H_ */
