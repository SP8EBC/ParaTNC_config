/*
 * GetRunningConfig.h
 *
 *  Created on: Aug 21, 2022
 *      Author: mateusz
 */

#ifndef SRC_SERVICES_SRVGETRUNNINGCONFIG_H_
#define SRC_SERVICES_SRVGETRUNNINGCONFIG_H_

#include "serial/Serial.h"
#include "../types/CurrentConfigRegion.h"

#include "../config/decode/IConfigDecode.h"
#include "../config/decode/IConfigValidate.h"

#include "IService.h"

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
#include <windows.h>
#else
#include <pthread.h>
#endif

class SrvGetRunningConfig : public IService {

	/**
	 * Pointer to serial port context used to talk with TNC
	 */
	Serial * s;

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	/**
	 *	Event used to synchronize threads and wait for all configuration to be
	 *	received from TNC
	 */
	HANDLE syncEvent;
#else
	/**
	 *	Condition variable used to synchronize threads and wait for all configuration to be
	 *	received from TNC
	 */
	pthread_cond_t * conditionVariable;
#endif

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
	virtual void sendRequest();

	virtual void receiveSynchronously();

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

#if defined (_MSC_VER) && (_MSC_VER <= 1400)

#else
	void setConditionVariable(pthread_cond_t * conditionVariable) {
		this->conditionVariable = conditionVariable;
	}
#endif

	const std::vector<uint8_t>& getConfigurationData() const {
		return configurationData;
	}
};

#endif /* SRC_SERVICES_SRVGETRUNNINGCONFIG_H_ */
