/*
 * Rtu.h
 *
 *  Created on: Aug 28, 2022
 *      Author: mateusz
 */

#ifndef SRC_TYPES_RTU_H_
#define SRC_TYPES_RTU_H_

#include <stdint.h>

class Rtu {
public:
	uint8_t busAddress;
	uint8_t function;
	uint16_t registerAddress;
	uint16_t lenght;
	uint8_t coeff_a;
	uint8_t coeff_b;
	uint8_t coeff_c;
	uint8_t coeff_d;
	bool unsignedSigned;
};

#endif /* SRC_TYPES_RTU_H_ */
