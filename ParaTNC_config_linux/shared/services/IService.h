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

#include "../shared/kiss_communication_nrc_t.h"

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

//	/**
//	 * Callback used if NRC is received from device or a timeout occured
//	 * @param nrc value received from device or NRC_NOT_DEFINED_BUT_TIMEOUT in case of timeout
//	 * @param isFromBackgroundAsyncThread true if it is called from @link{SerialRxBackgroundWorker}
//	 */
//	virtual void nrcCallback(const enum kiss_communication_nrc_t nrc, bool isFromBackgroundAsyncThread) = 0;


	/**
	 * @brief sends prepared request to the TNC. It is assumed that either a content of the
	 * request is already prepared, or it is fixed as per protocol definition
	 */
	virtual void sendRequest() = 0;
	
	/**
	 * @brief To be used after sending a request to a TNC, to lock calling thread while waiting
	 * for TNC and response, and automatically call a callback after this response is received
	 * @note callback is called from a context if this function
	 * @param cbk called in case of Negative Response Code is received 
	 */
	virtual void receiveSynchronously(IService_NegativeResponseCodeCbk cbk) = 0;
};

#endif /* SRC_SERVICES_ISERVICE_H_ */
