/*
 * How data are exchanged between Diagnostics Services callbacks and Serial I/O
 *
 * Requests from PC to tnc
 * =======================
 * Each class, which implements single diagnostics service has either a method, which construct
 * the content of a frame, or the content of this frame is fixed per definition (like RESET).
 * The content is kept in a vector of uint8_t, which is passed as reference to method
 * @link{transmitKissFrame} from Serial class. Data order in this vector looks like that
 * 		- Diagnostic service ID
 * 		- Service specific data
 * Inside the @link{transmitKissFrame} the diagnostic request is put inside KISS protocol.
 * At the TNC side it looks like that:
 * 	- FEND (0xC0) - *(input_frame_from_host + 0)
 * 	- frame_type / diagnostic service ID - *(input_frame_from_host + 1)
 * 	- service specific data....... like: 	uint16_t did = *(input_frame_from_host + 2) | (*(input_frame_from_host + 3) << 8);
 *
 *
 * Response from tnc to PC
 * =======================
 * Data sent back by TNC has slightly different format. Serial I/O is performed by a call to
 * @link{receiveKissFrame} from Serial class. Either synchronously from implementation of
 * interface method @link{receiveSynchronously}, or from background thread SerialRxBackgroundWorker.
 * Received data is stored in vector of uint8_t, which is passed as reference to @link{receiveKissFrame}.
 * Structure as seen on RS232 serial port looks like that:
 * 	- FEND (0xC0)
 * 	- NONSTANDARD (0x0F)
 * 	- response size - uint8_t
 * 	- diagnostic service response ID (service + 0x40)
 *	- specific data
 * Structure as seen in vector of bytes
 * 	- response size - uint8_t
 * 	- diagnostic service response ID - uint8_t - this->responseData[1];
 * 	- specific data
 *
 *	example of specific data taken from readDID
  		// rewind first two bytes which contains frame size and service ID
		it += 2;

		const uint8_t did_lsb = *it;
		it++;

		const uint8_t did_msb = *it;
		it++;

		this->didResponse.did = did_lsb | (did_msb << 8);
 *
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
