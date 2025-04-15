/*
 * LogDumper.h
 *
 *  Created on: Nov 8, 2024
 *      Author: mateusz
 */

#ifndef SRC_LOGDUMPER_H_
#define SRC_LOGDUMPER_H_

#include <string>

#include "LogDumperTextFile.h"
#include "serial/SerialRxBackgroundWorker.h"
#include "../shared/services/SrvReadMemory.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "../shared/event_log.h"
#ifdef __cplusplus
}
#endif


class LogDumper {

	/**
	 * Enum describes internal machine state of routines used to create a text
	 * file report
	 */
	enum LogDumper_TextFileStatus {
		NOT_STARTED,     /**< NOT_STARTED */
		STARTED,         /**< STARTED */
		WRITEN_AND_CLOSED/**< WRITEN_AND_CLOSED */
	};

	SrvReadMemory& srvReadMemory;
	pthread_cond_t& cond1;

	SerialRxBackgroundWorker &serialRxBackgroundWorker;

	/**
	 * Class writing report
	 */
	LogDumperTextFile logDumperTextFile;

	bool timeout;

	void timeoutCallback(void);

	static const uint16_t daysInYearByMonth[2][13];

	static void getTimestampFromEvent(const event_log_exposed_t * const in, struct tm * out);

protected:

	static bool convertEventToExposedEvent(const event_log_t * event, event_log_exposed_t & exposed, struct tm & decodedDateTime);


public:

	LogDumper(SrvReadMemory& _srvReadMemory, pthread_cond_t& _cond1, SerialRxBackgroundWorker& _serial_thread);
	virtual ~LogDumper();

	void dumpEventsToReport(uint32_t startAddress, uint32_t endAddress, std::string filename);
};

#endif /* SRC_LOGDUMPER_H_ */
