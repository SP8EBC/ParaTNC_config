/*
 * AuxStuff.h
 *
 *  Created on: Nov 5, 2023
 *      Author: mateusz
 */

#ifndef SRC_AUXSTUFF_H_
#define SRC_AUXSTUFF_H_

#include <string>
#include <stdint.h>

#include "../shared/kiss_communication_nrc_t.h"

class AuxStuff {
public:

	static std::string nrcToString(uint16_t supposedValue) {
		switch (supposedValue) {
		case NRC_GENERAL_REJECT: 			return "NRC_GENERAL_REJECT";
		case NRC_SERVICE_NOT_SUPPORTED: 	return "NRC_SERVICE_NOT_SUPPORTED";
		case NRC_SUBFUNCTION_NOT_SUPPORTED: return "NRC_SUBFUNCTION_NOT_SUPPORTED";
		case NRC_INCORRECT_MESSAGE_LENGTH_OR_FORMAT: return "NRC_INCORRECT_MESSAGE_LENGTH_OR_FORMAT";
		case NRC_RESPONSE_TOO_LONG:			return "NRC_RESPONSE_TOO_LONG";
		case NRC_CONDITIONS_NOT_CORRECT:	return "NRC_CONDITIONS_NOT_CORRECT";
		case NRC_REQUEST_OUT_OF_RANGE: 		return "NRC_REQUEST_OUT_OF_RANGE";
		case NRC_SECURITY_ACCESS_DENIED:	return "NRC_SECURITY_ACCESS_DENIED";
		}

		return "UNKNOWN";
	}

	AuxStuff();
	virtual ~AuxStuff();
};

#endif /* SRC_AUXSTUFF_H_ */
