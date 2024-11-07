/*
 * IConfigValidate.h
 *
 *  Created on: Aug 27, 2022
 *      Author: mateusz
 */

#ifndef SRC_CONFIG_DECODE_ICONFIGVALIDATE_H_
#define SRC_CONFIG_DECODE_ICONFIGVALIDATE_H_

#include <vector>
#include <stdint.h>

class IConfigValidate {
public:
	virtual ~IConfigValidate() {

	}

	virtual bool checkValidate(std::vector<uint8_t> & dataFromTnc) = 0;

};

#endif /* SRC_CONFIG_DECODE_ICONFIGVALIDATE_H_ */
