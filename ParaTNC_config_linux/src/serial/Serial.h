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
#include <array>

#include "../types/SerialState.h"

/**
 * Due to some unfortunate omission there is a inconsistency in KISS extented protocol.
 * Originally KISS frame with data received from RF channel looks like that:
 * 		FEND, 0x00, data, data, data, data, FEND
 * 	There is no frame ln anywhere. The second byte (first after FEND) is divided into two
 * 	nibbles. High nibble is an id of TNC port which RX/TX the data, low nibble is a command.
 *
 * 	In this KISS protocol extension (or rather variant) 0x00 meand always data from/to radio channel.
 * 	As there is always one, single radio port all other features of KISS proto all scraped. If second byte
 * 	is non zero it means that this is an exntented frame and this byte is keeps frame lenght. BUT THIS
 * 	ONLY APPLIES TO FRAMES TNC -> PC
 *
 * 	frames in opposite direction doesn't have size and second byte holds command ID
 *
 *
 */

using namespace std;

class Serial {

	SerialState serialState;

	int handle;

	struct termios tty;
	struct termios tty_old;

	std::array<uint8_t, 2048> raw;
	int i;


public:
	bool init(string port, speed_t speed);
	void testTransmit();

	void waitForTransmissionDone();

	void transmitKissFrame(std::shared_ptr<std::vector<uint8_t>> frame);
	void receiveKissFrame(std::shared_ptr<std::vector<uint8_t>> frame);

	Serial();
	virtual ~Serial();
};

#endif /* SERIAL_SERIAL_H_ */
