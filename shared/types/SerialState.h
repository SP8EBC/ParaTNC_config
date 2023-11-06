/*
 * SerialState.h
 *
 *  Created on: Aug 21, 2022
 *      Author: mateusz
 */

#ifndef SRC_TYPES_SERIALSTATE_H_
#define SRC_TYPES_SERIALSTATE_H_

typedef enum SerialState {
	SERIAL_NOT_CONFIGURED,
	SERIAL_IDLE,
	SERIAL_TRANSMITTING
}SerialState;

#endif /* SRC_TYPES_SERIALSTATE_H_ */
