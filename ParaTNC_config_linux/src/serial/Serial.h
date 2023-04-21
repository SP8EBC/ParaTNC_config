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
#include <stdint.h>

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
 * 	is non zero it means that this is an extended frame and this byte is keeps frame lenght. BUT THIS
 * 	ONLY APPLIES TO FRAMES TNC -> PC
 *
 * 	frames in opposite direction doesn't have size and second byte holds command ID
 *
 *
 */

#define SERIAL_RAW_ARRAY_SIZE	2048

/**
 * Class implementing communication through serial port
 */
class Serial {

	/**
	 * Current state of serial prot
	 */
	SerialState serialState;

	/**
	 * Handle to serial port
	 */
	int handle;

	/**
	 * Linux stuff
	 */
	struct termios tty;
	struct termios tty_old;

	/**
	 * Array which holds data received from device connected to serial port
	 */
	uint8_t raw[SERIAL_RAW_ARRAY_SIZE];

	/**
	 * Iterator used to go through array of raw data
	 */
	int rawArrayIterator;

	const static uint8_t FEND[1];		//!< FEND control byte
	const static uint8_t FESC[1];		//!< FESC control byte
	const static uint8_t TFEND[1];		//!< TFEND control byte
	const static uint8_t TFESC[1];		//!< TFESC control byte


public:

	/**
	 * Initializes and opens serial port
	 * @param port
	 * @param speed
	 * @return
	 */
	bool init(std::string port, speed_t speed);
	void testTransmit();

	/**
	 * Flush serial port FIFO and synchronously wait for all bytes to be sent
	 * on serial port
	 */
	void waitForTransmissionDone();

	/**
	 * Transmits KISS frame through serial port. Trailing and leading FEND are
	 * automatically added
	 * @param frame
	 */
	void transmitKissFrame(const std::vector<uint8_t> & frame);

	/**
	 * Synchronously waits and receives so called >>extended<< kiss frame
	 * @param frame
	 */
	void receiveKissFrame(std::vector<uint8_t> & frame);

	Serial();
	virtual ~Serial();
};

#endif /* SERIAL_SERIAL_H_ */
