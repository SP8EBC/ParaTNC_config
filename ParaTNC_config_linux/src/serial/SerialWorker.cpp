/*
 * SerialWorker.cpp
 *
 *  Created on: Aug 22, 2022
 *      Author: mateusz
 */

#include "SerialWorker.h"
#include "../services/ServicesIds.h"
#include <pthread.h>

#include <iostream>


SerialWorker::SerialWorker(Serial * serial, std::map<uint8_t, IService*> * callbcks)
																	: ctx(serial),
																	  callbackMap(callbcks)
{
	workerStartSync = PTHREAD_COND_INITIALIZER;

	workerLock = PTHREAD_MUTEX_INITIALIZER;

	thread = -1;

	pointerThis = this;

	workerLoop = true;

}

SerialWorker::~SerialWorker() {
}

SerialWorker& SerialWorker::operator=(const SerialWorker &other) {
	return * this;
}

void SerialWorker::waitForStartup(void) {

}

void * SerialWorker::wrapper(void * object) {

	SerialWorker * pointer = static_cast<SerialWorker *>(object);

	pointer->worker();

	return NULL;
}


void SerialWorker::worker(void) {
	std::cout << "I = SerialWorker::worker, start " << std::endl;

	pthread_cond_signal(&this->workerStartSync);

	// this vector is stripped from FEND!!!
	//std::shared_ptr<std::vector<uint8_t>> pointerToData = std::shared_ptr<std::vector<uint8_t>>(&receivedData);

	// pointer to callback
	IService * serviceCallback = NULL;

	do {
		// clean buffer
		receivedData.clear();

		// receive KISS packet from controller
		ctx->receiveKissFrame(receivedData);

		// check if anything has been recieved
		if (receivedData.size() > 0) {
			// get frame type
			uint8_t frameType = receivedData.at(1);

			serviceCallback = this->callbackMap->at(frameType);

			if (serviceCallback != NULL) {
				// invoke callback
				serviceCallback->callback(&receivedData);
			}
		}
	}	while (workerLoop);

	std::cout << "I = SerialWorker::worker, end " << std::endl;


}

bool SerialWorker::start(void) {

	if (callbackMap != NULL) {
		workerLoop = true;

		pthread_create(&this->thread, NULL, &SerialWorker::wrapper, (void*)&pointerThis);

		std::cout << "I = SerialWorker::start, started and sychronized " << std::endl;

		return true;
	}
	else {
		return false;
	}


}

void SerialWorker::terminate(void) {
	pthread_join(this->thread, NULL);

}
