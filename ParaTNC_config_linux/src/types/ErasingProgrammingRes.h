/*
 * ErasingProgrammingRes.h
 *
 *  Created on: Sep 11, 2022
 *      Author: mateusz
 */

#ifndef SRC_TYPES_ERASINGPROGRAMMINGRES_H_
#define SRC_TYPES_ERASINGPROGRAMMINGRES_H_


typedef enum erasing_programming_result_t {
	RESULT_IDLE		= 0xAA,
	RESULT_PENDING	= 0xAB,
	RESULT_ERASED	= 0xAC,
	RESULT_ERROR	= 0xAD
}erasing_programming_result_t;


#endif /* SRC_TYPES_ERASINGPROGRAMMINGRES_H_ */
