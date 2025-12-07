/*
 * ValidateVer0.h
 *
 *  Created on: Aug 27, 2022
 *      Author: mateusz
 */

#ifndef SRC_CONFIG_DECODE_VALIDATEVER0_H_
#define SRC_CONFIG_DECODE_VALIDATEVER0_H_

#include "IConfigValidate.h"

class ValidateVer0: public IConfigValidate {
public:
	virtual bool checkValidate(std::vector<uint8_t> & dataFromTnc);

	virtual bool recalculateChecksum(std::vector<uint8_t> & dataFromTnc);

	virtual ~ValidateVer0();
};

#endif /* SRC_CONFIG_DECODE_VALIDATEVER0_H_ */
