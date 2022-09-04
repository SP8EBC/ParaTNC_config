
#include "Serial.h"

#include <iostream>
#include <ios>
#include <stdint.h>
#include <cstring>
#include <vector>
#include <string>
#include <array>
#include <sys/time.h>

#include "../ProgramConfig.h"

#include "../exceptions/TimeoutE.h"
#include "../exceptions/TransmissionFailedEx.h"

#include "../types/ReceivingState.h"

	#define FEND	(uint8_t)0xC0
	#define FESC	(uint8_t)0xDB
	#define TFEND	(uint8_t)0xDC
	#define TFESC	(uint8_t)0xDD

using namespace std;

Serial::Serial() : serialState(SERIAL_NOT_CONFIGURED), i(0) {
}

void Serial::transmitKissFrame(std::shared_ptr<std::vector<uint8_t> > frame) {

	ssize_t transmissionResult;

	if (serialState == SERIAL_NOT_CONFIGURED) {
		return;
	}

	std::cout << "I = serial::transmitKissFrame, frame size: " << frame->size() << std::endl;

	if (frame && this->serialState == SERIAL_IDLE) {
		// send FEND at begining
		write(handle, std::array<uint8_t, 1>{FEND}.data(), 1);

		// send the content itself
		for (auto byte = frame->cbegin(); byte != frame->cend(); byte++) {
			transmissionResult = write(handle, &(*byte), 1);

			if (transmissionResult == 0) {
				std::cout << "E = serial::transmitKissFrame, error has occured while sending: 0x" << std::hex << *byte << std::dec << std::endl;

				throw TransmissionFailedEx();
			}
		}

		// send FEND at the end
		write(handle, std::array<uint8_t, 1>{FEND}.data(), 1);

		std::cout << "I = serial::transmitKissFrame, transmission done " << std::endl;
	}
}

void Serial::receiveKissFrame(std::shared_ptr<std::vector<uint8_t> > frame) {
	struct timeval receivingStart, currentTime;

	ReceivingState receivingState = RX_ST_WAITING_FOR_FEND;

	if (serialState == SERIAL_NOT_CONFIGURED) {
		return;
	}

	// received byte
	uint8_t rxData = 0;

	// return from read function
	ssize_t rxLn = 0;

	// amount of data between
	int16_t expectedRxLength = 0;

	if (frame) {
		// get a time when reception start
		gettimeofday(&receivingStart, NULL);

		do {
			// get current time
			gettimeofday(&currentTime, NULL);

			// try to receice single byte
			rxLn = read(handle, &rxData, 1);

			if (rxLn == 0) {
				continue;
			}

			raw[i++]	= rxData;

			// check if timeout
			if (currentTime.tv_sec - receivingStart.tv_sec > 1) {
				std::cout << "E = serial::receiveKissFrame, timeout has occured. " << std::endl;

				continue;
				//throw TimeoutE();
			}

			if (receivingState == RX_ST_STARTED) {
				// decrement amont of data to receive
				expectedRxLength--;

				// check if all bytes has been received
				if (expectedRxLength <= 0) {
					receivingState = RX_ST_DONE;
					//std::cout << "I = serial::receiveKissFrame, receiving done, frame->size(): " << frame->size() << std::endl;
					// do not place the last byte as this is always FEND
					if (rxData != FEND) {
						std::cout << "E = serial::receiveKissFrame, the last byte is 0x" << std::hex << (int)rxData << std::dec << " not 0xC0 (FEND). " << std::endl;

					}
				}
				else {
					// add data to output buffer
					frame->push_back(rxData);

					if (rxData == FEND) {
						std::cout << "E = serial::receiveKissFrame, unexpected 0xC0 (FEND)! current expectedRxLength: " << expectedRxLength << ", i: " << i << std::endl;

					}
				}
			}

			// the next byte after FEND holds a frame size (from FEND to FEND)
			if (receivingState == RX_ST_STARTED_WAITING_FOR_LN) {
				expectedRxLength = rxData - 2;		// exclude FEND at the start and this byte
				receivingState = RX_ST_STARTED;

				std::cout << "D = serial::receiveKissFrame, expectedRxLength: " << expectedRxLength << std::endl;

				frame->push_back(rxData);
			}

			if (receivingState == RX_ST_WAITING_FOR_FEND && rxData == FEND) {
				receivingState = RX_ST_STARTED_WAITING_FOR_LN;
			}
		} while(receivingState != RX_ST_DONE);

	}
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
