
#include "Serial.h"

#include <iostream>
#include <ios>
#include <stdint.h>
#include <cstring>
#include <vector>
#include <string>
#include <sys/time.h>

#include <serial/serial.h>

#include "../shared/exceptions/TimeoutE.h"
#include "../shared/exceptions/TransmissionFailedEx.h"

#include "../shared/types/ReceivingState.h"

#define _FEND	(uint8_t)0xC0
#define _FESC	(uint8_t)0xDB
#define _TFEND	(uint8_t)0xDC
#define _TFESC	(uint8_t)0xDD

#define _NONSTANDARD	(uint8_t)0x0F

#define FRAME_LN_OFFSET 2

extern bool verboseLogging;

/**
 * Control byte which denotes start and end of a frame
 */
const uint8_t Serial::FEND[] = {_FEND};

/**
 * Escape byte if FEND byte must be send as a data, not as signalling
 */
const uint8_t Serial::FESC[] = {_FESC};

/**
 * Escaped FEND control byte, sent after FESC if FEND is to be sent as data.
 */
const uint8_t Serial::TFEND[] = {_TFEND};

/**
 *
 */
const uint8_t Serial::TFESC[] = {_TFESC};

Serial::Serial() : serialState(SERIAL_NOT_CONFIGURED), rawArrayIterator(0), timeouts(0) {
	memset (raw, 0x00, SERIAL_RAW_ARRAY_SIZE);
}

/**
 * Transmits KISS frame through serial port. Trailing and leading FEND are
 * automatically added
 * @param frame
 */
void Serial::transmitKissFrame(const std::vector<uint8_t> & frame) {

	size_t transmissionResult = 0;

	// check if serial port is opened and configured
	if (serialState == SERIAL_NOT_CONFIGURED) {
		return;
	}

	//std::cout << "I = serial::transmitKissFrame, frame size: " << frame.size() << std::endl;

	if (this->serialState == SERIAL_IDLE) {

		// buffer with everything what will be pushed to the serial port. The content
		// is escaped on the way in, so it can be sent by a single call to the library
		std::vector<uint8_t> toTransmit;

		// send FEND at begining
		toTransmit.push_back(*Serial::FEND);

		// send the content itself
		for (std::vector<uint8_t>::const_iterator it = frame.begin(); it != frame.end(); it++) {

			// get byte fron the iterator
			const uint8_t byte = *it;

			switch (byte) {
				case _FEND:
					toTransmit.push_back(*Serial::FESC);
					toTransmit.push_back(*Serial::TFEND);
					break;
				case _FESC:
					toTransmit.push_back(*Serial::FESC);
					toTransmit.push_back(*Serial::TFESC);
					break;
				default: {
					// no special action needed
					// put this byte into the buffer as-is
					toTransmit.push_back(byte);
					break;
				}
			}
		}

		// send FEND at the end
		toTransmit.push_back(*Serial::FEND);

		try {
			transmissionResult = this->port->write(toTransmit);
		}
		catch (const std::exception & e) {
			std::cout << "E = serial::transmitKissFrame, error has occured while sending: " << e.what() << std::endl;

			throw TransmissionFailedEx();
		}

		// check if everything has been pushed out to the serial port
		if (transmissionResult != toTransmit.size()) {
			std::cout << "E = serial::transmitKissFrame, only " << transmissionResult << " bytes out of " << toTransmit.size() << " have been sent" << std::endl;

			throw TransmissionFailedEx();
		}

		//std::cout << "D = serial::transmitKissFrame, transmission done " << std::endl;
	}
}

/**
 * Synchronously waits and receives so called >>extended<< kiss frame. It
 * returns when a complete frame is received. It is called by SerialWorker
 * (separate thread) in a loop one received frame after another.
 * Internally it checks for a timeout in case that communication with
 * the controlled stalled for some reason.
 * @param frame
 */
void Serial::receiveKissFrame(std::vector<uint8_t> & frame) {
	struct timeval receivingStart, currentTime;

	ReceivingState receivingState = RX_ST_WAITING_FOR_FEND;

	if (serialState == SERIAL_NOT_CONFIGURED) {
		std::cout << "E = serial::receiveKissFrame, serial port not configured" << std::endl;
		return;
	}

	// received byte
	uint8_t rxData = 0;

	// how many bytes the library has returned from a single read
	size_t rxLn = 0;

	// amount of data between
	int16_t expectedRxLength = 0;

	// get a time when reception start
	gettimeofday(&receivingStart, NULL);

	// zero
	memset (raw, 0x00, SERIAL_RAW_ARRAY_SIZE);
	do {
		// get current time
		gettimeofday(&currentTime, NULL);

		// try to receive single byte
		try {
			rxLn = this->port->read(&rxData, 1);
		}
		catch (const std::exception & e) {
			std::cout << "E = serial::receiveKissFrame, error has occured while receiving: " << e.what() << std::endl;

			throw TransmissionFailedEx();
		}

		// no data has been received
		if (rxLn == 0) {
			//std::cout << "W = serial::receiveKissFrame" << std::endl;

			// check if timeout, as without this the loop would spin here
			// forever if the controller stops talking at all
			if (currentTime.tv_sec - receivingStart.tv_sec > 10) {
				timeouts++;
				std::cout << "E = serial::receiveKissFrame, timeout has occured for " << timeouts << " time" << std::endl;

				throw TimeoutE();
			}

			if (verboseLogging) {
				std::cout << "---- serial::receiveKissFrame" << std::endl;
			}

			// continue the loop
			continue;
		}

		// put received data into
		raw[rawArrayIterator++]	= rxData;
		if (rawArrayIterator >= SERIAL_RAW_ARRAY_SIZE - 1) {
			rawArrayIterator = 0;
		}

		// check if timeout
		// TODO: must be fixed, the method shall returns
		// with an error in case of timeout instead of looping here
		// for no sense.
		if (currentTime.tv_sec - receivingStart.tv_sec > 10) {
			timeouts++;
			std::cout << "E = serial::receiveKissFrame, timeout has occured for " << timeouts << " time" << std::endl;

			throw TimeoutE();
			//continue;
		}

		if (receivingState == RX_ST_STARTED) {
			// decrement amont of data to receive
			expectedRxLength--;

			// check if all bytes has been received
			if (expectedRxLength <= 0) {
				receivingState = RX_ST_DONE;
				//std::cout << "I = serial::receiveKissFrame, receiving done, frame->size(): " << frame->size() << std::endl;
				// do not place the last byte as this is always FEND
				if (rxData != *FEND) {
					std::cout << "E = serial::receiveKissFrame, the last byte is 0x" << std::hex << (int)rxData << std::dec << " not 0xC0 (FEND). " << std::endl;

				}
			}
			else {
				// add data to output buffer
				frame.push_back(rxData);

				if (rxData == *FEND) {
					std::cout << "E = serial::receiveKissFrame, unexpected 0xC0 (FEND)! current expectedRxLength: " << expectedRxLength << ", i: " << rawArrayIterator << std::endl;

				}
			}
		}

		// the next byte after NONSTANDARD holds a frame size (from FEND to FEND)
		if (receivingState == RX_ST_STARTED_WAITING_FOR_LN) {
			expectedRxLength = rxData - 3;		// exclude FEND at the start and this byte
			receivingState = RX_ST_STARTED;

			//std::cout << "D = serial::receiveKissFrame, expectedRxLength: " << expectedRxLength << std::endl;

			frame.push_back(rxData);
		}

		if (receivingState == RX_ST_STARTED_WAITING_FOR_NONSTANDARD) {
			if (rxData == _NONSTANDARD) {
				receivingState = RX_ST_STARTED_WAITING_FOR_LN;
			}
		}

		if (receivingState == RX_ST_WAITING_FOR_FEND && rxData == *FEND) {
			receivingState = RX_ST_STARTED_WAITING_FOR_NONSTANDARD;
		}
	} while(receivingState != RX_ST_DONE);


}

Serial::~Serial() {
	// unique_ptr closes and destroys the port from the library
}

/**
 * Initializes and opens serial port
 * @param portName
 * @param baudrate	baudrate as a plain number of bits per second, like 9600
 * @return
 */
bool Serial::init(std::string portName, uint32_t baudrate)
{
	// timeouts used by the library. Inter byte timeout is disabled, a single
	// read waits SERIAL_READ_TIMEOUT_MSEC at most and then returns with
	// whatever has been received (which might be nothing at all)
	serial::Timeout portTimeouts (serial::Timeout::max(),
								  SERIAL_READ_TIMEOUT_MSEC,
								  0,
								  SERIAL_WRITE_TIMEOUT_MSEC,
								  SERIAL_WRITE_TIMEOUT_PER_BYTE_MSEC);

	try {
		// create the port. 8 data bits, no parity, one stop bit and no flow
		// control are the library defaults and this is exactly what is needed here
		this->port.reset (new serial::Serial ());

		this->port->setPort (portName);
		this->port->setBaudrate (baudrate);
		this->port->setTimeout (portTimeouts);
		this->port->setBytesize (serial::eightbits);
		this->port->setParity (serial::parity_none);
		this->port->setStopbits (serial::stopbits_one);
		this->port->setFlowcontrol (serial::flowcontrol_none);

		this->port->open();
	}
	catch (const std::exception & e) {
		std::cout << "E = serial::init, cannot open serial port " << portName << ", " << e.what() << std::endl;

		this->port.reset();

		return false;
	}

	if (!this->port->isOpen()) {
		std::cout << "E = serial::init, serial port " << portName << " is not opened" << std::endl;

		this->port.reset();

		return false;
	}

	// get rid of anything the operating system could buffer before the port was opened
	this->port->flushInput();

	this->serialState = SERIAL_IDLE;

	std::cout << "I = serial::init, serial port " << portName << " has been configured" << std::endl;

	return true;
}

void Serial::waitForTransmissionDone() {

	if (serialState == SERIAL_NOT_CONFIGURED) {
		return;
	}

	// this waits until everything written to the port is physically sent out
	this->port->flush();
}
