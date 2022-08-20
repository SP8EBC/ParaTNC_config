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

using namespace std;

class serial {

	int handle;

	struct termios tty;
	struct termios tty_old;

public:
	void init(string port);
	void testTransmit();

	void transmitKissFrame(std::shared_ptr<std::vector<uint8_t>> frame);
	void receiveKissFrame(std::shared_ptr<std::vector<uint8_t>> frame);

	serial();
	virtual ~serial();
};

#endif /* SERIAL_SERIAL_H_ */
