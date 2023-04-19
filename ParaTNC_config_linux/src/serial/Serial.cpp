
#include "Serial.h"

#include <iostream>
#include <ios>
#include <stdint.h>
#include <cstring>
#include <vector>
#include <string>
#include <sys/time.h>

#include "../ProgramConfig.h"

#include "../exceptions/TimeoutE.h"
#include "../exceptions/TransmissionFailedEx.h"

#include "../types/ReceivingState.h"

#define _FEND	(uint8_t)0xC0
#define _FESC	(uint8_t)0xDB
#define _TFEND	(uint8_t)0xDC
#define _TFESC	(uint8_t)0xDD

#define _NONSTANDARD	(uint8_t)0x0F

#define FRAME_LN_OFFSET 2

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

Serial::Serial() : serialState(SERIAL_NOT_CONFIGURED), rawArrayIterator(0) {
}

void Serial::transmitKissFrame(std::vector<uint8_t> & frame) {

	ssize_t transmissionResult;

	// check if serial port is opened and configured
	if (serialState == SERIAL_NOT_CONFIGURED) {
		return;
	}

	std::cout << "I = serial::transmitKissFrame, frame size: " << frame.size() << std::endl;

	if (this->serialState == SERIAL_IDLE) {
		// send FEND at begining
		write(handle, FEND, 1);

		// send the content itself
		for (std::vector<uint8_t>::iterator it = frame.begin(); it != frame.end(); it++) {

			// get byte fron the iterator
			const uint8_t byte = *it;

			// write this byte to the serial port
			transmissionResult = write(handle, &byte, 1);

			// check if eror has
			if (transmissionResult == 0) {
				std::cout << "E = serial::transmitKissFrame, error has occured while sending: 0x" << std::hex << byte << std::dec << std::endl;

				throw TransmissionFailedEx();
			}
		}

		// send FEND at the end
		write(handle, FEND, 1);

		std::cout << "I = serial::transmitKissFrame, transmission done " << std::endl;
	}
}

void Serial::receiveKissFrame(std::vector<uint8_t> & frame) {
	struct timeval receivingStart, currentTime;

	ReceivingState receivingState = RX_ST_WAITING_FOR_FEND;

	if (serialState == SERIAL_NOT_CONFIGURED) {
		return;
	}

	// received byte
	uint8_t rxData = 0;

	// return from read functionbyte
	ssize_t rxLn = 0;

	// amount of data between
	int16_t expectedRxLength = 0;

	// get a time when reception start
	gettimeofday(&receivingStart, NULL);

	// zero
	memset (raw, 0x00, SERIAL_RAW_ARRAY_SIZE);
	do {
		// get current time
		gettimeofday(&currentTime, NULL);

		// try to receice single byte
		rxLn = read(handle, &rxData, 1);

		// no data has been received
		if (rxLn == 0) {
			// continue the loop
			continue;
		}

		// put received data into
		raw[rawArrayIterator++]	= rxData;
		if (rawArrayIterator >= SERIAL_RAW_ARRAY_SIZE - 1) {
			rawArrayIterator = 0;
		}

		// check if timeout
		if (currentTime.tv_sec - receivingStart.tv_sec > 1) {
			std::cout << "E = serial::receiveKissFrame, timeout has occured. " << std::endl;

			continue;
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

			std::cout << "D = serial::receiveKissFrame, expectedRxLength: " << expectedRxLength << std::endl;

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
	// TODO Auto-generated destructor stub
}

bool Serial::init(string port, speed_t speed)
{
	struct termios tty;
	struct termios tty_old;
	memset (&tty, 0, sizeof tty);

	handle = open( port.c_str(), O_RDWR| O_NOCTTY );


	/* Error Handling */
	if ( tcgetattr ( handle, &tty ) != 0 ) {
		std::cout << "E = serial::init, tcgetattr error: " << strerror(errno) << std::endl;

		return false;
	}

	/* Save old tty parameters */
	tty_old = tty;

	/* Set Baud Rate */

	tty.c_iflag &= ~(IMAXBEL|IXOFF|INPCK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON|IGNPAR);
	tty.c_iflag |= IGNBRK;

	tty.c_oflag &= ~OPOST;
	tty.c_oflag &= ~CRTSCTS;

	tty.c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHONL|ICANON|ISIG|IEXTEN|NOFLSH|TOSTOP|PENDIN);
	tty.c_cflag &= ~(CSIZE|PARENB);
	tty.c_cflag |= CS8|CREAD;
	tty.c_cc[VMIN] = 0;		// bylo 80
	tty.c_cc[VTIME] = 3;		// byo 3

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	/* Make raw */
//	cfmakeraw(&tty);

	/* Flush Port, then applies attributes */
	tcflush( handle, TCIFLUSH );
	if ( tcsetattr ( handle, TCSANOW, &tty ) != 0) {
		std::cout << "E = serial::init, tcsetattr error: " << strerror(errno) << std::endl;
	   //std::cout << "Error " << errno << " from tcsetattr" << std::endl;

		return false;
	}

	this->serialState = SERIAL_IDLE;

	std::cout << "I = serial::init, serial port " << port << " has been configured" << std::endl;

	return true;
}

void Serial::waitForTransmissionDone() {
	tcdrain(handle);
}
//void serial::test_transmit()
//{
//	for (uint8_t i = 0; i < 255; i++)
//		write( handle, &i, 1 );
//}
//
//short serial::checkCRC(char* pInputData) {
//	char ii,i = 0;
//	unsigned short crc = 0xFFFF;
//	ii = pInputData[6] + 12;
//	for (i = 0; i < ii - 3; i++)
//		crc = calc_crc(crc, pInputData[i]);
//	if ( (pInputData[ii - 2] == ( (crc & 0xFF00) >> 8) ) && ( pInputData[ii - 3] == (crc & 0xFF) ) )
//		return 0;
//	else
//		return -1;
//}
//
//unsigned short serial::calc_crc(unsigned short crc_buff, unsigned char input) {
//	unsigned char i;
//	unsigned short x16;
//	for	(i=0; i<8; i++)
//	{
//		// XOR current D0 and next input bit to determine x16 value
//		if		( (crc_buff & 0x0001) ^ (input & 0x01) )
//			x16 = 0x8408;
//		else
//			x16 = 0x0000;
//		// shift crc buffer
//		crc_buff = crc_buff >> 1;
//		// XOR in the x16 value
//		crc_buff ^= x16;
//		// shift input for next iteration
//		input = input >> 1;
//	}
//	return (crc_buff);
//}
