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

class SrvSendStartupConfig: public IService {

public:
	/**
	 * Sends a request to download and flash startup configuration. A call to
	 * this function will send only first part of configuration block.
	 * The rest will be handled by callback
	 */
	void sendRequest();

	SrvSendStartupConfig(std::vector<uint8_t> & configData);
	virtual ~SrvSendStartupConfig();
	SrvSendStartupConfig(const SrvSendStartupConfig &other);
	SrvSendStartupConfig(SrvSendStartupConfig &&other);
	SrvSendStartupConfig& operator=(const SrvSendStartupConfig &other);
	SrvSendStartupConfig& operator=(SrvSendStartupConfig &&other);

	virtual void callback(
			const std::vector<unsigned char, std::allocator<unsigned char> > &frame)
					override;
};

#endif /* SRC_SERVICES_SRVSENDSTARTUPCONFIG_H_ */
