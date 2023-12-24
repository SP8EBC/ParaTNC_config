
#include "stdafx.h"
#include "Serial.h"
#include "../shared/types/ReceivingState.h"
#include "../shared/exceptions/TimeoutE.h"

#include <iostream>

#define _FEND	(uint8_t)0xC0
#define _FESC	(uint8_t)0xDB
#define _TFEND	(uint8_t)0xDC
#define _TFESC	(uint8_t)0xDD

#define _NONSTANDARD	(uint8_t)0x0F

#define FRAME_LN_OFFSET 2

#define SERIAL_MAXIMUM_RX_TIME_MS	500U

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

const wchar_t* Serial::COM1 = L"\\\\.\\COM1";
const wchar_t* Serial::COM2 = L"\\\\.\\COM2";
const wchar_t* Serial::COM3 = L"\\\\.\\COM3";
const wchar_t* Serial::COM4 = L"\\\\.\\COM4";
const wchar_t* Serial::COM5 = L"\\\\.\\COM5";
const wchar_t* Serial::COM6 = L"\\\\.\\COM6";

// https://stackoverflow.com/questions/25100736/writing-to-serial-port-is-blocking-forever-when-other-thread-is-waiting-in-read
// stupid windows cannot read and write to the serial port at once :(
// calling WriteFile while a thread waits on ReadFile will create a deadlock :/

Serial::Serial() {
	serialPort = INVALID_HANDLE_VALUE;
	rawArrayIterator = 0;
	memset(raw, 0x00, SERIAL_RAW_ARRAY_SIZE);
	std::cout << "D = Serial::Serial" << std::endl;
}


Serial::~Serial() {
	// TODO Auto-generated destructor stub
}

bool Serial::init()
{
	bool out = true;			// return value
	DCB serialParams = {0u};	// keeps port config, like speed, databits, parity etc.
	COMMTIMEOUTS tout = {0u};	// timeouts for read / write etc
	WCHAR errorMessage[256U];

	// try to open serial port
	serialPort = CreateFile(COM1, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);

	std::cout << "I = Serial::init" << std::endl;

	// check if open was successfull
	if (serialPort == INVALID_HANDLE_VALUE) {
		const DWORD lastError = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, lastError, NULL, errorMessage, 256U, NULL);
		std::wcout << "E = Serial::init, lastError: " << lastError << std::endl;
		out = false;
	}
	else {
		// don't really know why it is needed
		serialParams.DCBlength = sizeof(serialParams);

		// get current port configuration
		if (GetCommState(serialPort, &serialParams) == TRUE) {
			// set configuration to talk to ParaTNC / ParaMETEO
			serialParams.BaudRate = CBR_9600;
			serialParams.ByteSize = 8;
			serialParams.StopBits = ONESTOPBIT;
			serialParams.Parity = NOPARITY;
			serialParams.fDtrControl = DTR_CONTROL_DISABLE;
			serialParams.fOutX = FALSE;
			serialParams.fInX = FALSE;
			serialParams.fRtsControl = RTS_CONTROL_DISABLE;

			// set serial port configuration
			if (SetCommState(serialPort, &serialParams) == TRUE) {
				/**
				  *	The maximum time allowed to elapse before 
				  *	the arrival of the next byte on the communications 
				  *	line, in milliseconds. If the interval between 
				  *	the arrival of any two bytes exceeds this amount, 
				  *	the ReadFile operation is completed and any buffered 
				  *	data is returned. A value of zero indicates that 
				  *	interval time-outs are not used. 
				  *	
				  *	A value of MAXDWORD, combined with zero values for 
				  *	both the ReadTotalTimeoutConstant and ReadTotalTimeoutMultiplier 
				  *	members, specifies that the read operation is 
				  *	to return immediately with the bytes that have already 
				  *	been received, even if no bytes have been received.
				  *
				  *  https://learn.microsoft.com/en-gb/windows/win32/api/winbase/ns-winbase-commtimeouts?redirectedfrom=MSDN
				  */
				tout.ReadIntervalTimeout = (DWORD)1000u;
				tout.ReadTotalTimeoutConstant = (DWORD)1000u;
				tout.ReadTotalTimeoutMultiplier = (DWORD)10u;
				tout.WriteTotalTimeoutConstant = 0;
				tout.WriteTotalTimeoutMultiplier = 0;

				SetCommTimeouts(serialPort, &tout);

				if (SetCommMask(serialPort, EV_RXCHAR) == TRUE) {
					;
				}
				else {
					out = false;
				}
			}
			else {
				out = false;
			}
		}
		else {
			out = false;
		}
	}

	if (out) {
		serialState = SERIAL_IDLE;
			std::cout << "I = Serial::init, initialization succeded" << std::endl;
	}

	return out;
}

void Serial::transmitKissFrame(const std::vector<uint8_t> & frame) {

	DWORD numberOfBytesWritten;

	// check if serial port is opened and configured
	if (serialState == SERIAL_NOT_CONFIGURED) {
		return;
	}

	if (this->serialState == SERIAL_IDLE) {
		// send FEND at begining
		WriteFile(serialPort, FEND, 1, &numberOfBytesWritten, NULL);

		WriteFile(serialPort, &frame[0], frame.size(), &numberOfBytesWritten, NULL);

		WriteFile(serialPort, FEND, 1, &numberOfBytesWritten, NULL);

		std::cout << "I = serial::transmitKissFrame, transmission done " << std::endl;
	}
}

void Serial::receiveKissFrame(std::vector<uint8_t> & frame) {
	SYSTEMTIME receivingStart, currentTime;

	ReceivingState receivingState = RX_ST_WAITING_FOR_FEND;

	if (serialState == SERIAL_NOT_CONFIGURED) {
		//std::cout << "E = serial::receiveKissFrame, serial port not configured" << std::endl;
		return;
	}

	std::cout << "D = serial::receiveKissFrame, receiving started..." << std::endl;

	// received byte
	uint8_t rxData = 0;

	// return from read functionbyte
	DWORD rxLn = 0;

	// amount of data between
	int16_t expectedRxLength = 0;

	// get a time when reception start
	GetSystemTime(&receivingStart);

	// zero
	memset (raw, 0x00, SERIAL_RAW_ARRAY_SIZE);
	do {
		// block the thread and wait for any new data to be reveived
		// WARNING: this function has no timeout and waits indefinitely
		//const BOOL status = WaitCommEvent(serialPort, &eventMask, NULL);

		// try to read all byes available right now
		do {
			// get current time (in UTC)
			GetSystemTime(&currentTime);

			// try to receice single byte
			//rxLn = read(handle, &rxData, 1);
			const BOOL readResult = ReadFile(serialPort, &rxData, 1, &rxLn, NULL);

			// check if timeout
			// TODO: must be fixed, the method shall returns
			// with an error in case of timeout instead of looping here
			// for no sense.
			if (Serial::compareTime(receivingStart, currentTime) > SERIAL_MAXIMUM_RX_TIME_MS) {
				std::cout << "E = serial::receiveKissFrame, timeout has occured. " << std::endl;

				throw TimeoutE();
			}

			// no data has been received
			if (rxLn == 0 || readResult == FALSE) {
				std::cout << "W = serial::receiveKissFrame, rxLn: " << rxLn << ", readResult: " << readResult << std::endl;

				// continue the loop
				continue;
			}

			// put received data into
			raw[rawArrayIterator++]	= rxData;
			if (rawArrayIterator >= SERIAL_RAW_ARRAY_SIZE - 1) {
				rawArrayIterator = 0;
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
		} while(receivingState != RX_ST_DONE && rxLn > 0);
	} while (receivingState != RX_ST_DONE);
}

void Serial::waitForTransmissionDone() {

}
