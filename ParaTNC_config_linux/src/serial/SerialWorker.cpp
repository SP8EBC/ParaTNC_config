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
																	: ctx(serial), callbackMap(callbcks) {

}

SerialWorker::~SerialWorker() {
}

SerialWorker::SerialWorker(const SerialWorker &other) : callbackMap(other.callbackMap) {
}

SerialWorker::SerialWorker(SerialWorker &&other) : callbackMap(other.callbackMap) {

}

SerialWorker& SerialWorker::operator=(const SerialWorker &other) {

}

SerialWorker& SerialWorker::operator=(SerialWorker &&other) {

}

void * SerialWorker::wrapper(void * object) {

	SerialWorker** pointer = (SerialWorker**)object;

	auto callable = std::bind(&SerialWorker::worker, *pointer);

	callable();

	return NULL;
}


void SerialWorker::worker(void) {
	std::cout << "I = SerialWorker::worker, start ";

	std::shared_ptr<std::vector<uint8_t>> pointerToData = std::make_shared<std::vector<uint8_t>>();

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

			switch(frameType) {
				case KISS_RUNNING_CONFIG:
					serviceCallback = this->callbackMap.at(KISS_RUNNING_CONFIG);
					break;
				default: break;
			}

			if (serviceCallback != NULL) {
				// invoke callback
				serviceCallback->callback(receivedData);
			}
		}
	}	while (workerLoop);

	std::cout << "I = SerialWorker::worker, end ";


}

bool SerialWorker::start(void) {

	SerialWorker** pointerToObject = new SerialWorker*;

	*pointerToObject = this;

	workerLoop = true;

	pthread_create(&this->thread, NULL, &SerialWorker::wrapper, pointerToObject);

	return true;
}

void SerialWorker::terminate(void) {
	pthread_join(this->thread, NULL);

}
