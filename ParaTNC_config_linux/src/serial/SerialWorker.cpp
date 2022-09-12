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


SerialWorker::SerialWorker(std::shared_ptr<Serial> serial, std::map<uint8_t, IService*> & callbcks)
																	: ctx(serial), callbackMap(callbcks),
																	  workerStartSync(PTHREAD_COND_INITIALIZER),
																	  workerLock(PTHREAD_MUTEX_INITIALIZER) {
	pointerThis = std::shared_ptr<SerialWorker>(this);

	workerLoop = true;

}

SerialWorker::~SerialWorker() {
}

SerialWorker::SerialWorker(const SerialWorker &other) : callbackMap(other.callbackMap) {
}

SerialWorker::SerialWorker(SerialWorker &&other) : callbackMap(other.callbackMap) {

}

SerialWorker& SerialWorker::operator=(const SerialWorker &other) {
	return * this;
}

void SerialWorker::waitForStartup(void) {

}

SerialWorker& SerialWorker::operator=(SerialWorker &&other) {
	return * this;

}

void * SerialWorker::wrapper(void * object) {

	std::shared_ptr<SerialWorker> * pointer = (std::shared_ptr<SerialWorker> *)object;

	auto callable = std::bind(&SerialWorker::worker, *pointer);

	callable();

	return NULL;
}


void SerialWorker::worker(void) {
	std::cout << "I = SerialWorker::worker, start " << std::endl;

	pthread_cond_signal(&this->workerStartSync);

	// this vector is stripped from FEND!!!
	std::shared_ptr<std::vector<uint8_t>> pointerToData = std::shared_ptr<std::vector<uint8_t>>(&receivedData);

	// pointer to callback
	IService * serviceCallback = NULL;

	do {
		// clean buffer
		pointerToData->clear();

		// receive KISS packet from controller
		ctx->receiveKissFrame(pointerToData);

		// check if anything has been recieved
		if (pointerToData->size() > 0) {
			// get frame type
			uint8_t frameType = pointerToData->at(1);

			serviceCallback = this->callbackMap.at(frameType);

			if (serviceCallback != NULL) {
				// invoke callback
				serviceCallback->callback(*pointerToData);
			}
		}
	}	while (workerLoop);

	std::cout << "I = SerialWorker::worker, end " << std::endl;


}

bool SerialWorker::start(void) {

	workerLoop = true;

//    pthread_mutex_lock(&workerLock);

	pthread_create(&this->thread, NULL, &SerialWorker::wrapper, (void*)&pointerThis);

//    // wait for configuration to be received
//    pthread_cond_wait(&workerStartSync, &workerLock);
//
//    pthread_mutex_unlock(&workerLock);

	std::cout << "I = SerialWorker::start, started and sychronized " << std::endl;

	return true;
}

void SerialWorker::terminate(void) {
	pthread_join(this->thread, NULL);

}
