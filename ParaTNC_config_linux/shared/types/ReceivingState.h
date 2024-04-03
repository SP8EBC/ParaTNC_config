/*
 * ReceivingState.h
 *
 *  Created on: Aug 21, 2022
 *      Author: mateusz
 */

#ifndef SRC_TYPES_RECEIVINGSTATE_H_
#define SRC_TYPES_RECEIVINGSTATE_H_

typedef enum ReceivingState{
	RX_ST_WAITING_FOR_FEND,
	RX_ST_STARTED_WAITING_FOR_NONSTANDARD,
	RX_ST_STARTED_WAITING_FOR_LN,
	RX_ST_STARTED,
	RX_ST_DONE
}ReceivingState;

#endif /* SRC_TYPES_RECEIVINGSTATE_H_ */
