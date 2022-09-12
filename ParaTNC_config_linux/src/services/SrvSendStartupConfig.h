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

#include "../serial/Serial.h"

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
	std::shared_ptr<std::vector<uint8_t>> segmentedData;

	/**
	 * Current offset through data to be send to TNC. In other words this is
	 */
	uint16_t currentOffset;

	/**
	 * Pointer to serial port context used to talk with TNC
	 */
	std::shared_ptr<Serial> s;

public:
	/**
	 * Sends a request to download and flash startup configuration. A call to
	 * this function will send only first part of configuration block.
	 * The rest will be handled by callback
	 */
	void sendRequest();

	SrvSendStartupConfig(std::vector<uint8_t> && configData, int singleFrameLn);
	virtual ~SrvSendStartupConfig();
	SrvSendStartupConfig(const SrvSendStartupConfig &other);
	SrvSendStartupConfig(SrvSendStartupConfig &&other);
	SrvSendStartupConfig& operator=(const SrvSendStartupConfig &other);
	SrvSendStartupConfig& operator=(SrvSendStartupConfig &&other);

	virtual void callback(
			const std::vector<unsigned char, std::allocator<unsigned char> > &frame)
					override;

	void setSerialContext(const std::shared_ptr<Serial> &s) {
		this->s = s;
	}
};

#endif /* SRC_SERVICES_SRVSENDSTARTUPCONFIG_H_ */
