/*
 * RoutineControlResult.h
 *
 *  Created on: Jun 9, 2026
 *      Author: mateusz
 */

#ifndef SHARED_TYPES_ROUTINECONTROLRESULT_H_
#define SHARED_TYPES_ROUTINECONTROLRESULT_H_

#include <cstdint>

typedef struct RoutineControlResult {
	uint16_t routineId;	//!< which this result corresponds to
	uint8_t wasNrc;	//!< Set to non zero if a Routine Control request resulted in NRC
	uint16_t resultCode; //!< Routine result itself or NRC value
}RoutineControlResult;


#endif /* SHARED_TYPES_ROUTINECONTROLRESULT_H_ */
