/*
 * serial.h
 *
 *  Created on: 22.09.2017
 *      Author: mateusz
 */

#ifndef SERIAL_SERIAL_H_
#define SERIAL_SERIAL_H_

#include <string>

#include <memory>
#include <vector>
#include <stdint.h>
#include "windows.h"

#include "../shared/types/SerialState.h"

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
 * 	frames in opposite direction doesn't have size and second byte holds command ID, for more information
 *	plese look into file ./src/kiss_protocol/kiss_communication.c in ParaTNC source project.
 *
 *
 */

#define SERIAL_RAW_ARRAY_SIZE		2048

#define SERIAL_MILISECONDS_PER_DAY		(DWORD)86400000UL
#define SERIAL_MILISECONDS_PER_HOUR		(DWORD)3600000UL
#define SERIAL_MILISECONDS_PER_MINUTE	(DWORD)60000UL
#define SERIAL_MILISECONDS_PER_SECOND	(DWORD)1000UL

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
	HANDLE serialPort;

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

	const static wchar_t* COM1;
	const static wchar_t* COM2;
	const static wchar_t* COM3;
	const static wchar_t* COM4;
	const static wchar_t* COM5;
	const static wchar_t* COM6;

	/**
	 * Compares two SYSTEMTIME structures and returns difference in miliseconds.
	 * Positive number means that time represend by 'to' is in future in relation to 'from' 
	 */
	static DWORD compareTime(const SYSTEMTIME from, const SYSTEMTIME to) {
		DWORD fromMs = 0UL;
		DWORD toMs = 0UL;

		fromMs =	from.wDay * SERIAL_MILISECONDS_PER_DAY +
			from.wHour * SERIAL_MILISECONDS_PER_HOUR +
			from.wMinute * SERIAL_MILISECONDS_PER_MINUTE +
			from.wSecond * SERIAL_MILISECONDS_PER_SECOND +
			from.wMilliseconds;

		toMs = to.wDay * SERIAL_MILISECONDS_PER_DAY +
			to.wHour * SERIAL_MILISECONDS_PER_HOUR +
			to.wMinute * SERIAL_MILISECONDS_PER_MINUTE +
			to.wSecond * SERIAL_MILISECONDS_PER_SECOND +
			to.wMilliseconds;

		return toMs - fromMs;
	}

public:

	/**
	 * Initializes and opens serial port
	 * @param port
	 * @param speed
	 * @return
	 */
	bool init();
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
	 * Synchronously waits and receives so called >>extended<< kiss frame. It
	 * returns when a complete frame is received. It is called by SerialWorker
	 * (separate thread) in a loop one received frame after another.
	 * Internally it checks for a timeout in case that communication with
	 * the controlled stalled for some reason.
	 * @param frame
	 */
	void receiveKissFrame(std::vector<uint8_t> & frame);

	Serial();
	virtual ~Serial();
};

#endif /* SERIAL_SERIAL_H_ */
