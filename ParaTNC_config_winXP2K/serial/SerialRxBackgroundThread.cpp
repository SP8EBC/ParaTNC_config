#include "stdafx.h"

#include "./serial/SerialRxBackgroundThread.h"
#include "./misc/AuxStuff.h"
#include "../shared/exceptions/TimeoutE.h"
#include "../shared/kiss_communication_service_ids.h"

#include <iostream>

DWORD WINAPI SerialRxBackgroundThread::EntryPoint(LPVOID param) {

	if (param == NULL) {
		return 1;
	}

	std::cout << "I = SerialBackgroundThread::EntryPoint" << std::endl;

	LPSRBT ptr = static_cast<LPSRBT>(param);

	// pointer to callback function
	IService * callbackFunction = NULL;

	BOOL eventResult = SetEvent(ptr->startEvent);

	// remove all old data from vector
	ptr->rxData.clear();

	try {
		// synchronously receive kiss frame
		ptr->serial->receiveKissFrame(ptr->rxData);
	}
	catch (TimeoutE & ex) {
		// timeout while receiving a frame, something is wrong

		// exit from the main loop
		std::cout << "E = SerialWorker::worker, timeout" << std::endl;
		return 1;
	}
	if (ptr->rxData.size() > 1) {
		// get frame type, which was received
		uint8_t frameType = ptr->rxData[1];

		if (frameType == KISS_NEGATIVE_RESPONSE_SERVICE) {
			std::cout << "E = SerialWorker::worker, NRC received: " << AuxStuff::nrcToString(ptr->rxData[2]) << std::endl;
		}
		else {
			// look for a pointer to a callback function
			std::map<uint8_t, IService*>::const_iterator it = ptr->callbackMap->find(frameType);

			// if callack has been found
			if (it != ptr->callbackMap->end()) {
				callbackFunction = static_cast<IService *>(it->second);

				callbackFunction->callback(&ptr->rxData);
			}
		}
	}

	return 0;
}

bool SerialRxBackgroundThread::start() {

	// create background thread and start it immediately
	thread = CreateThread(NULL, 0, SerialRxBackgroundThread::EntryPoint, this, 0, NULL);

	WaitForSingleObject(startEvent, (DWORD)1234U);

	return true;
}

SerialRxBackgroundThread::SerialRxBackgroundThread(Serial * serialPort, std::map<uint8_t, IService*> * callbacks) {
	callbackMap = callbacks;
	serial = serialPort;
	startEvent = CreateEvent(NULL, FALSE, FALSE, L"SerialThreadStart");
	workerLoop = true;
}

SerialRxBackgroundThread::~SerialRxBackgroundThread() {

}
