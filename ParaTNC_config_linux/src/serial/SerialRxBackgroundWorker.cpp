/*
 * SerialWorker.cpp
 *
 *  Created on: Aug 22, 2022
 *      Author: mateusz
 */

#include "../AuxStuff.h"
#include "../shared/kiss_communication_service_ids.h"
#include "../shared/exceptions/TimeoutE.h"
#include <pthread.h>
#include <serial/SerialRxBackgroundWorker.h>

#include <iostream>


SerialRxBackgroundWorker::SerialRxBackgroundWorker(Serial * serial, std::map<uint8_t, IService*> callbcks, std::function<void (uint16_t)> _nrcCallback)
																	: ctx(serial),
																	  callbackMap(callbcks),
																	  nrcCallback(_nrcCallback)
{
	workerStartSync = PTHREAD_COND_INITIALIZER;

	workerLock = PTHREAD_MUTEX_INITIALIZER;

	thread = -1;

	pointerThis = this;

	workerLoop = true;

	workerStarted = false;

}

SerialRxBackgroundWorker::~SerialRxBackgroundWorker() {
}

SerialRxBackgroundWorker& SerialRxBackgroundWorker::operator=(const SerialRxBackgroundWorker &other) {
	return * this;
}

void SerialRxBackgroundWorker::waitForStartup(void) {

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

void * SerialRxBackgroundWorker::wrapper(void * object) {

	SerialRxBackgroundWorker * pointer = static_cast<SerialRxBackgroundWorker *>(object);

	pointer->worker();

	return NULL;
}


void SerialRxBackgroundWorker::worker(void) {
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

		try {
			// receive KISS packet from controller
			ctx->receiveKissFrame(receivedData);

			// check if anything has been recieved
			if (receivedData.size() > 0) {
				// get frame type
				uint8_t frameType = receivedData.at(1);

				if (frameType == KISS_NEGATIVE_RESPONSE_SERVICE) {
					std::cout << "E = SerialWorker::worker, NRC received: " << AuxStuff::nrcToString(receivedData.at(2)) << std::endl;
					this->nrcCallback(receivedData.at(2));
				}
				else {
					serviceCallback = this->callbackMap.at(frameType);

					if (serviceCallback != NULL) {
						// invoke callback
						serviceCallback->callback(&receivedData);
					}
				}

			}
		}
		catch (TimeoutE & ex) {
			if (this->backgroundTimeoutCallback) {
				this->backgroundTimeoutCallback();
			}
		}
	}	while (workerLoop);

	std::cout << "I = SerialWorker::worker, end " << std::endl;


}

bool SerialRxBackgroundWorker::start(void) {

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
			pthread_create(&this->thread, NULL, &SerialRxBackgroundWorker::wrapper, (void*)pointerThis);

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

void SerialRxBackgroundWorker::terminate(void) {
	pthread_cancel(thread);
	pthread_join(this->thread, NULL);

}
