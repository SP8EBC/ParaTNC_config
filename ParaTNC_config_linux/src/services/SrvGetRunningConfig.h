/*
 * GetRunningConfig.h
 *
 *  Created on: Aug 21, 2022
 *      Author: mateusz
 */

#ifndef SRC_SERVICES_SRVGETRUNNINGCONFIG_H_
#define SRC_SERVICES_SRVGETRUNNINGCONFIG_H_

#include "../serial/Serial.h"
#include "../types/CurrentConfigRegion.h"

#include "../config/decode/IConfigDecode.h"
#include "../config/decode/IConfigValidate.h"

#include "IService.h"

class SrvGetRunningConfig : public IService {

	/**
	 * Pointer to serial port context used to talk with TNC
	 */
	Serial * s;

	/**
	 *	Condition variable used to synchronize threads and wait for all configuration to be
	 *	received from TNC
	 */
	pthread_cond_t * conditionVariable;

	/**
	 * Current memory area which is received from the TNC + and information if any reception is
	 * initiated. This holds an internal state
	 */
	CurrentConfigRegion currentRegion;

	/**
	 * How many frames is expected to be received from TNC
	 */
	uint16_t expectedKissFrames;

	/**
	 * Configuration data extracted from frames
	 */
	std::vector<uint8_t> configurationData;

	IConfigValidate * validate;

	void reset();

	static const std::vector<uint8_t> requestData;
public:
	/**
	 * Sends a request to upload current running configuration
	 */
	void sendRequest();

	/**
	 * Callback to be used when a portion of configuration data
	 * is received from TNC by host pc
	 */
	virtual void callback(const std::vector<uint8_t> * frame);

	/**
	 * Save configuration into binary file
	 * @param _in filename
	 * @return
	 */
	bool storeToBinaryFile(std::string _in);

	SrvGetRunningConfig();
	virtual ~SrvGetRunningConfig();
	SrvGetRunningConfig(const SrvGetRunningConfig &other);
	SrvGetRunningConfig& operator=(const SrvGetRunningConfig &other);

	void setSerialContext(Serial * s) {
		this->s = s;
	}

	void setConditionVariable(pthread_cond_t * conditionVariable) {
		this->conditionVariable = conditionVariable;
	}

	const std::vector<uint8_t>& getConfigurationData() const {
		return configurationData;
	}
};

#endif /* SRC_SERVICES_SRVGETRUNNINGCONFIG_H_ */
