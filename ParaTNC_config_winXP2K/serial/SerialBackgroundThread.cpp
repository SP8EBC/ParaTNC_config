#include "stdafx.h"

#include "./serial/SerialBackgroundThread.h"
#include "../shared/exceptions/TimeoutE.h"
#include "../shared/kiss_communication_service_ids.h"

DWORD WINAPI SerialBackgroundThread::EntryPoint(LPVOID param) {

	if (param == NULL) {
		return 1;
	}

	SerialBackgroundThread * ptr = static_cast<SerialBackgroundThread*>(param);

	// pointer to callback function
	IService * callbackFunction = NULL;

	SetEvent(ptr->startEvent);

	do {
		// remove all old data from vector
		ptr->rxData.clear();

		try {
			// synchronously receive kiss frame
			ptr->serial->receiveKissFrame(ptr->rxData);
		}
		catch (TimeoutE & ex) {
			// timeout while receiving a frame, something is wrong

			// exit from the main loop
			ptr->workerLoop = false;
			continue;
		}
		
		// get frame type, which was received
		uint8_t frameType = ptr->rxData[1];

		if (frameType == KISS_NEGATIVE_RESPONSE_SERVICE) {
			//std::cout << "E = SerialWorker::worker, NRC received: " << AuxStuff::nrcToString(receivedData.at(2)) << std::endl;
		}
		else {
			// look for a pointer to a callback function
			std::map<uint8_t, IService*>::const_iterator it = ptr->callbackMap.find(frameType);

			// if callack has been found
			if (it != ptr->callbackMap.end()) {
				callbackFunction = static_cast<IService *>(it->second);

				callbackFunction->callback(&ptr->rxData);
			}


			//if (callbackFunction != NULL) {
			//	// invoke callback
			//	serviceCallback->callback(&ptr->rxData);
			//}
		}
	} while (ptr->workerLoop);

	return 0u;
}

bool SerialBackgroundThread::start() {

	// create background thread and start it immediately
	thread = CreateThread(NULL, 0, SerialBackgroundThread::EntryPoint, this, 0, NULL);

	return true;
}

SerialBackgroundThread::SerialBackgroundThread() {
	serial = new Serial();
	startEvent = CreateEvent(NULL, FALSE, FALSE, L"SerialThreadStart");
	workerLoop = true;
}

SerialBackgroundThread::~SerialBackgroundThread() {

}
