/*
 * IService.h
 *
 *  Created on: Aug 22, 2022
 *      Author: mateusz
 */

#ifndef SRC_SERVICES_ISERVICE_H_
#define SRC_SERVICES_ISERVICE_H_

#include <vector>
#include <stdint.h>

#define NONSTANDARD	(uint8_t)0x0F

typedef void(*IService_NegativeResponseCodeCbk)(uint16_t nrc) ;

class IService {

public:

	virtual ~IService();

	/**
	 * Callback invoked after successful reception of full frame from TNC. Please note that a
	 * vector which is passed here doesn't have FEND at the first and the last position.
	 */
	virtual void callback(const std::vector<uint8_t> * frame) = 0;


	virtual void sendRequest() = 0;

	virtual void receiveSynchronously(IService_NegativeResponseCodeCbk cbk) = 0;
};

#endif /* SRC_SERVICES_ISERVICE_H_ */
