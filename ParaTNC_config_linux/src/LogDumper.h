/*
 * LogDumper.h
 *
 *  Created on: Nov 8, 2024
 *      Author: mateusz
 */

#ifndef SRC_LOGDUMPER_H_
#define SRC_LOGDUMPER_H_

#include <string>

#include "../shared/services/SrvReadMemory.h"
#include "EventDescriptionsStorage.hpp"
#include "LogDumperTextFile.h"
#include "serial/SerialRxBackgroundWorker.h"

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
		NOT_STARTED,	  /**< NOT_STARTED */
		STARTED,		  /**< STARTED */
		WRITEN_AND_CLOSED /**< WRITEN_AND_CLOSED */
	};

	SrvReadMemory &srvReadMemory;
	sem_t &cond1;

	SerialRxBackgroundWorker &serialRxBackgroundWorker;

	/**
	 * Class writing a report to text file
	 */
	LogDumperTextFile logDumperTextFile;

	std::unique_ptr<EventDescriptionsStorage> jsonEvents;

	bool timeout;

	bool decodeFromJsonOnly;

	int progress;

	void timeoutCallback (void);

	static const uint16_t daysInYearByMonth[2][13];

	static void getTimestampFromEvent (const event_log_exposed_t *const in, struct tm *out);

  protected:
	static bool convertEventToExposedEvent (const event_log_t *event, event_log_exposed_t &exposed,
											struct tm &decodedDateTime);

  public:
	LogDumper (SrvReadMemory &_srvReadMemory, sem_t &_cond1,
			   SerialRxBackgroundWorker &_serial_thread,
			   std::unique_ptr<EventDescriptionsStorage> &&_jsonEvents, bool _decodeJsonOnly);
	virtual ~LogDumper ();

	void dumpEventsToReport (uint32_t startAddress, uint32_t endAddress, std::string filename,
							 bool exitOnFirstCrcFail);
};

#endif /* SRC_LOGDUMPER_H_ */
