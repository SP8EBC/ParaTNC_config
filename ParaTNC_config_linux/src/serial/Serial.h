/*
 * serial.h
 *
 *  Created on: 22.09.2017
 *      Author: mateusz
 */

#ifndef SERIAL_SERIAL_H_
#define SERIAL_SERIAL_H_

#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <string>

#include <memory>
#include <vector>

#include "../types/SerialState.h"

using namespace std;

class Serial {

	SerialState serialState;

	int handle;

	struct termios tty;
	struct termios tty_old;



public:
	bool init(string port, speed_t speed);
	void testTransmit();

	void transmitKissFrame(std::shared_ptr<std::vector<uint8_t>> frame);
	void receiveKissFrame(std::shared_ptr<std::vector<uint8_t>> frame);

	Serial();
	virtual ~Serial();
};

#endif /* SERIAL_SERIAL_H_ */
