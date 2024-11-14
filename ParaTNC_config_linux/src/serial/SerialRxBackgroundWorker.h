/*
 * SerialWorker.h
 *
 *  Created on: Aug 22, 2022
 *      Author: mateusz
 */

#ifndef SRC_SERIAL_SERIALRXBACKGROUNDWORKER_H_
#define SRC_SERIAL_SERIALRXBACKGROUNDWORKER_H_

#include <memory>
#include <functional>
#include <map>

#include "Serial.h"

#include "../shared/services/IService.h"

/**
 * This is serial worker class which is used to receive data
 * in background thread
 */
class SerialRxBackgroundWorker {

	/**
	 * Serial used to receive data in background
	 */
	Serial * ctx;

	/**
	 * Handle to thread worker
	 */
	pthread_t thread;

	/**
	 *
	 */
	pthread_cond_t workerStartSync;

	pthread_mutex_t workerLock;

	/**
	 * Static wrapper to workaround static call to pthread_create
	 */
	static void * wrapper(void * object);

	/**
	 * Worker itself which is called by the wrapper
	 */
	void worker(void);

	/**
	 * Here worker will place data received from controller
	 */
	std::vector<uint8_t> receivedData;

	/**
	 * Map pointing to
	 */
	std::map<uint8_t, IService*> callbackMap;

	bool workerLoop;

	/**
	 * Set to true by worker after startup
	 */
	bool workerStarted;

	SerialRxBackgroundWorker * pointerThis;

public:
	/**
	 * Optional pointer to a callback, which is used by background thread in
	 * case a timeout
	 */
	void(*backgroundTimeoutCallback)(void) = NULL;

	bool start(void);

	void terminate(void);

	void waitForStartup(void);

	SerialRxBackgroundWorker(Serial * serial, std::map<uint8_t, IService*> callbcks);
	virtual ~SerialRxBackgroundWorker();

	SerialRxBackgroundWorker& operator=(const SerialRxBackgroundWorker &other);
};

#endif /* SRC_SERIAL_SERIALRXBACKGROUNDWORKER_H_ */
