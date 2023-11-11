
#include "stdafx.h"
#include "Serial.h"

#include <iostream>

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

Serial::Serial() {

}

void Serial::transmitKissFrame(const std::vector<uint8_t> & frame) {

		std::cout << "I = serial::transmitKissFrame, transmission done " << std::endl;
}

void Serial::receiveKissFrame(std::vector<uint8_t> & frame) {

}

Serial::~Serial() {
	// TODO Auto-generated destructor stub
}

bool Serial::init()
{
	return true;
}

void Serial::waitForTransmissionDone() {

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
