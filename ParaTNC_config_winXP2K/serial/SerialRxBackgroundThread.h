#pragma once

#include "./serial/Serial.h"
#include "../shared/services/IService.h"
#include <windows.h>

#include <map>

/**
  * This class is a wrapper over 'Serial' class (which implements
  * serial communication). This wrapper handles receiving data
  * from serial port and calls a callback when correct KISS frame
  * is received
  *
 */
class SerialRxBackgroundThread
{
private:

	// pointer to serial handling class, which contains methods
	// to perform serial I/O, receive and transmit KISS frames etc.
	// generally methods inside Serial class has blocking I/O
	// hence RXing must be separated to external thread, which
	// will work in background, not to block 
	Serial * serial;

	// handle to thread 
	HANDLE thread;

	// an event generated when serial thread starts
	HANDLE startEvent;

	// set to false to stop and exit from background thread
	bool workerLoop;

	// vector with bytes received through serial port
	std::vector<uint8_t> rxData;

	// map between service ID in kiss frame and callbacks
	std::map<uint8_t, IService*> * callbackMap;

	static DWORD WINAPI EntryPoint(LPVOID param);
public:
	bool start();

	SerialRxBackgroundThread(Serial * serialPort, std::map<uint8_t, IService*> * callbacks);
	~SerialRxBackgroundThread(void);

};

typedef SerialRxBackgroundThread SRBT;
typedef SerialRxBackgroundThread * LPSRBT;
