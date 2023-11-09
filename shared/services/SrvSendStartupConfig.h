/*
 * SrvSendStartupConfig.h
 *
 *  Created on: Sep 8, 2022
 *      Author: mateusz
 */

#ifndef SRC_SERVICES_SRVSENDSTARTUPCONFIG_H_
#define SRC_SERVICES_SRVSENDSTARTUPCONFIG_H_

#include "IService.h"

#include <vector>
#include <memory>

#include "./serial/Serial.h"
#include "../types/ErasingProgrammingRes.h"
#include "../types/NRC.h"


class SrvSendStartupConfig: public IService {

	const int singleFrameLn;

	/**
	 * This is complete data to be send to the TNC
	 */
	std::vector<uint8_t> dataForDownload;

	/**
	 * This vector holds single chunk of config data to be transferred to TNC,
	 * just a single KISS frame
	 */
	std::vector<uint8_t>  segmentedData;

	/**
	 * Current offset through data to be send to TNC. In other words this is
	 */
	uint16_t currentOffset;

	/**
	 * Pointer to serial port context used to talk with TNC
	 */
	Serial * s;

	/**
	 * Condition variable used to synchronize and lock another thread until
	 * memory erase is done. TNC sends ACK after flash operation is done.
	 */
	pthread_cond_t * conditionVariable;

	/**
	 * This condition variable is used to lock flashing thread internally until
	 * the TNC is ready to flash next block of configuration data
	 * 	 */
	pthread_cond_t internalSync;

	/**
	 *
	 */
	kiss_communication_nrc_t operationResult;

public:
	/**
	 * Sends a request to download and flash startup configuration. A call to
	 * this function will send only first part of configuration block.
	 * The rest will be handled by callback
	 */
	virtual void sendRequest();

	SrvSendStartupConfig(int singleFrameLn);
	virtual ~SrvSendStartupConfig();
	SrvSendStartupConfig(const SrvSendStartupConfig &other);
	SrvSendStartupConfig& operator=(const SrvSendStartupConfig &other);

	virtual void callback(
			const std::vector<unsigned char, std::allocator<unsigned char> > *frame);

	void setSerialContext( Serial * s) {
		this->s = s;
	}

	void setDataForDownload(std::vector<uint8_t> & _dataForDownload) {
		this->dataForDownload = std::vector<uint8_t>(_dataForDownload);
	}

	void setConditionVariable(
			pthread_cond_t * conditionVariable) {
		this->conditionVariable = conditionVariable;
	}
};

#endif /* SRC_SERVICES_SRVSENDSTARTUPCONFIG_H_ */
