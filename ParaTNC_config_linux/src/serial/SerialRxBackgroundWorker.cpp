/*
 * SerialWorker.cpp
 *
 *  Created on: Aug 22, 2022
 *      Author: mateusz
 */

#include "../AuxStuff.h"
#include "../shared/exceptions/TimeoutE.h"
#include "../shared/kiss_communication_service_ids.h"
#include <semaphore.h>
#include <serial/SerialRxBackgroundWorker.h>

#include <iostream>

SerialRxBackgroundWorker::SerialRxBackgroundWorker (Serial *serial,
													std::map<uint8_t, IService *> callbcks,
													std::function<void (uint16_t)> _nrcCallback)
	: ctx (serial), callbackMap (callbcks), nrcCallback (_nrcCallback)
{
	workerLock = PTHREAD_MUTEX_INITIALIZER;

	thread = -1;

	pointerThis = this;

	workerLoop = true;

	workerStarted = false;
}

SerialRxBackgroundWorker::~SerialRxBackgroundWorker ()
{
}

SerialRxBackgroundWorker &
SerialRxBackgroundWorker::operator= (const SerialRxBackgroundWorker &other)
{
	return *this;
}

void SerialRxBackgroundWorker::waitForStartup (void)
{

	// check if worker is running already
	if (workerStarted) {
		// and return immediately if is.
		return;
	}

	// if not wait on condition variable
	pthread_mutex_lock (&this->workerLock);
	sem_wait (&this->workerStartSync);
	pthread_mutex_unlock (&this->workerLock);
}

void *SerialRxBackgroundWorker::wrapper (void *object)
{

	SerialRxBackgroundWorker *pointer = static_cast<SerialRxBackgroundWorker *> (object);

	pointer->worker ();

	return NULL;
}

void SerialRxBackgroundWorker::worker (void)
{
	std::cout << "I = SerialWorker::worker, start " << std::endl;

	// signalize a waiting thread that this worker has started
	// pthread_mutex_lock (&this->workerLock);			// TODO::
	sem_post (&this->workerStartSync);
	// pthread_mutex_unlock (&this->workerLock);		// TODO::???

	// set flag which is then used by waiting thread to check if worker
	// had started before that thread
	workerStarted = true;

	// pointer to callback
	IService *serviceCallback = NULL;

	do {
		// clean buffer
		receivedData.clear ();
		uint8_t frameType = 0;

		try {
			// receive KISS packet from controller
			ctx->receiveKissFrame (receivedData);

			// check if anything has been recieved
			if (receivedData.size () > 0) {
				// get frame type
				frameType = receivedData.at (1);

				if (frameType == KISS_NEGATIVE_RESPONSE_SERVICE) {
					std::cout << "E = SerialWorker::worker, NRC received: "
							  << AuxStuff::nrcToString (receivedData.at (2)) << std::endl;
					this->nrcCallback (receivedData.at (2));
				}
				else {
					serviceCallback = this->callbackMap.at (frameType);

					if (serviceCallback != NULL) {
						// invoke callback
						serviceCallback->callback (&receivedData);
					}
				}
			}
		}
		catch (TimeoutE &ex) {
			std::cout << "E = SerialWorker::worker, TIMEOUT" << std::endl;
			if (this->backgroundTimeoutCallback) {
				this->backgroundTimeoutCallback ();
			}
		}
		catch (std::out_of_range &e) {
			std::cout << "E = SerialWorker::worker, std::out_of_range exception!! frameType: 0x"
					  << std::hex << (int)frameType << std::endl;
		}
	} while (workerLoop);

	std::cout << "I = SerialWorker::worker, end " << std::endl;
}

bool SerialRxBackgroundWorker::start (void)
{

	// check if callback have been set
	if (callbackMap.size () > 0) {
		// set to keep worker looping
		workerLoop = true;

		// initialize mutex
		const int mutex_init_result = pthread_mutex_init (&workerLock, NULL);

		// initialize semaphore
		//		If  pshared  has the value 0, then the semaphore is shared between the threads of a
		// process, 		and should be located at some address that is visible 		to all
		// threads (e.g., a global variable, or a variable allocated dynamically on the heap).

		//		If pshared is nonzero, then the semaphore is shared between processes, and should be
		// located 		in  a  region  of  shared  memory  (see  shm_open(3), mmap(2), and
		// shmget(2)). 		(Since a child created by fork(2) inherits its parent's memory mappings,
		// it
		// can also access the semaphore.)  Any process that can access the shared memory region can
		// operate on the semaphore using sem_post(3), sem_wait(3), and so on.

		const int cond_init_result = sem_init (&workerStartSync, (int)0, (unsigned int)0);

		// check and proceed only if all things were initialized correctly
		if (cond_init_result == 0 && mutex_init_result == 0) {
			// create and start working thread.
			pthread_create (&this->thread,
							NULL,
							&SerialRxBackgroundWorker::wrapper,
							(void *)pointerThis);

			// wait for thread to statup and became ready
			waitForStartup ();

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

void SerialRxBackgroundWorker::terminate (void)
{
	pthread_cancel (thread);
	pthread_join (this->thread, NULL);
}
