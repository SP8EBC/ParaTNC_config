/*
 * SerialWorker.cpp
 *
 *  Created on: Aug 22, 2022
 *      Author: mateusz
 */

#include "SerialWorker.h"
#include "../AuxStuff.h"
#include "../shared/kiss_communication_service_ids.h"
#include <pthread.h>

#include <iostream>


SerialWorker::SerialWorker(Serial * serial, std::map<uint8_t, IService*> callbcks)
																	: ctx(serial),
																	  callbackMap(callbcks)
{
	workerStartSync = PTHREAD_COND_INITIALIZER;

	workerLock = PTHREAD_MUTEX_INITIALIZER;

	thread = -1;

	pointerThis = this;

	workerLoop = true;

	workerStarted = false;

}

SerialWorker::~SerialWorker() {
}

SerialWorker& SerialWorker::operator=(const SerialWorker &other) {
	return * this;
}

void SerialWorker::waitForStartup(void) {

	// check if worker is running already
	if (workerStarted) {
		// and return immediately if is.
		return;
	}

	// if not wait on condition variable
    pthread_mutex_lock(&this->workerLock);
    pthread_cond_wait(&this->workerStartSync, &this->workerLock);
    pthread_mutex_unlock(&this->workerLock);
}

void * SerialWorker::wrapper(void * object) {

	SerialWorker * pointer = static_cast<SerialWorker *>(object);

	pointer->worker();

	return NULL;
}


void SerialWorker::worker(void) {
	std::cout << "I = SerialWorker::worker, start " << std::endl;

	// signalize a waiting thread that this worker has started
    pthread_mutex_lock(&this->workerLock);
	pthread_cond_signal(&this->workerStartSync);
    pthread_mutex_unlock(&this->workerLock);

    // set flag which is then used by waiting thread to check if worker
    // had started before that thread
    workerStarted = true;

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

			if (frameType == KISS_NEGATIVE_RESPONSE_SERVICE) {
				std::cout << "E = SerialWorker::worker, NRC received: " << AuxStuff::nrcToString(receivedData.at(2)) << std::endl;
			}
			else {
				serviceCallback = this->callbackMap.at(frameType);

				if (serviceCallback != NULL) {
					// invoke callback
					serviceCallback->callback(&receivedData);
				}
			}

		}
	}	while (workerLoop);

	std::cout << "I = SerialWorker::worker, end " << std::endl;


}

bool SerialWorker::start(void) {

	// check if callback have been set
	if (callbackMap.size() > 0) {
		// set to keep worker looping
		workerLoop = true;

		// initialize mutex
		const int mutex_init_result = pthread_mutex_init(&workerLock, NULL);

		// initialize condition variable
		const int cond_init_result = pthread_cond_init(&workerStartSync, NULL);

		// check and proceed only if all things were initialized correctly
		if (cond_init_result == 0 && mutex_init_result == 0) {
			// create and start working thread.
			pthread_create(&this->thread, NULL, &SerialWorker::wrapper, (void*)pointerThis);

			// wait for thread to statup and became ready
			waitForStartup();

			std::cout << "I = SerialWorker::start, started and sychronized " << std::endl;

			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}


}

void SerialWorker::terminate(void) {
	pthread_join(this->thread, NULL);

}
