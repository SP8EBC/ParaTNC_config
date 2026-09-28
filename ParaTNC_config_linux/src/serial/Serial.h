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
#include <stdint.h>
#include <vector>

#include "../shared/types/SerialState.h"

/**
 * Serial port from wjwwood library, which does all operating system specific
 * stuff internally. It is only forward declared here to keep the library
 * include path (and the library header itself) local to Serial.cpp
 */
namespace serial {
class Serial;
}

/**
 * Due to some unfortunate omission there is a inconsistency in KISS extented protocol.
 * Originally KISS frame with data received from RF channel looks like that:
 * 		FEND, 0x00, data, data, data, data, FEND
 * 	There is no frame ln anywhere. The second byte (first after FEND) is divided into two
 * 	nibbles. High nibble is an id of TNC port which RX/TX the data, low nibble is a command.
 *
 * 	In this KISS protocol extension (or rather variant) 0x00 meand always data from/to radio
 * channel. As there is always one, single radio port all other features of KISS proto all scraped.
 * If second byte is non zero it means that this is an extended frame and this byte is keeps frame
 * lenght. BUT THIS ONLY APPLIES TO FRAMES TNC -> PC
 *
 * 	frames in opposite direction doesn't have size and second byte holds command ID
 *
 *
 */

#define SERIAL_RAW_ARRAY_SIZE 2048

/**
 * How long (in milliseconds) a single read call waits for data before it
 * gives up and returns with nothing received
 */
#define SERIAL_READ_TIMEOUT_MSEC 300u

/**
 * Constant part of a timeout (in milliseconds) used for transmission
 */
#define SERIAL_WRITE_TIMEOUT_MSEC 1000u

/**
 * Per byte part of a transmission timeout (in milliseconds). Ten milliseconds
 * for each byte is way more than enough even for slowest baudrates used here
 */
#define SERIAL_WRITE_TIMEOUT_PER_BYTE_MSEC 10u

/**
 * Class implementing communication through serial port
 */
class Serial {

	/**
	 * Current state of serial prot
	 */
	SerialState serialState;

	/**
	 * Serial port itself. All operating system specific calls are done
	 * by the library, this class only uses its portable API
	 */
	std::unique_ptr<serial::Serial> port;

	/**
	 * Array which holds data received from device connected to serial port
	 */
	uint8_t raw[SERIAL_RAW_ARRAY_SIZE];

	/**
	 * Iterator used to go through array of raw data
	 */
	int rawArrayIterator;

	/**
	 * How many timeouts have been detected so far
	 */
	int timeouts;

	const static uint8_t FEND[1];  //!< FEND control byte
	const static uint8_t FESC[1];  //!< FESC control byte
	const static uint8_t TFEND[1]; //!< TFEND control byte
	const static uint8_t TFESC[1]; //!< TFESC control byte

  public:
	/**
	 * Initializes and opens serial port
	 * @param portName
	 * @param baudrate	baudrate as a plain number of bits per second, like 9600
	 * @return
	 */
	bool init (std::string portName, uint32_t baudrate);
	void testTransmit ();

	/**
	 * Flush serial port FIFO and synchronously wait for all bytes to be sent
	 * on serial port
	 */
	void waitForTransmissionDone ();

	/**
	 * Transmits KISS frame through serial port. Trailing and leading FEND are
	 * automatically added
	 * @param frame
	 */
	void transmitKissFrame (const std::vector<uint8_t> &frame);

	/**
	 * Synchronously waits and receives so called >>extended<< kiss frame. It
	 * returns when a complete frame is received. It is called by SerialWorker
	 * (separate thread) in a loop one received frame after another.
	 * Internally it checks for a timeout in case that communication with
	 * the controlled stalled for some reason.
	 * @param frame
	 */
	void receiveKissFrame (std::vector<uint8_t> &frame);

	Serial ();
	virtual ~Serial ();
};

#endif /* SERIAL_SERIAL_H_ */
