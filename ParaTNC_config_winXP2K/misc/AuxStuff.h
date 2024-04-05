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

#include "../shared/types/NRC.h"

class AuxStuff {
public:

	static std::string nrcToString(uint16_t supposedValue) {
		switch (supposedValue) {
		case NRC_REQUEST_OUT_OF_RANGE: return "NRC_REQUEST_OUT_OF_RANGE";
		case NRC_SERVICE_NOT_SUPPORTED: return "NRC_SERVICE_NOT_SUPPORTED";
		}
		return "UNKNOWN";
	}

	static int getResourceIdForNrc(uint16_t nrc);

	AuxStuff();
	virtual ~AuxStuff();
};

#endif /* SRC_AUXSTUFF_H_ */
