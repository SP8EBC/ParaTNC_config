/*
 * LogDumperTextFile.h
 *
 *  Created on: Nov 13, 2024
 *      Author: mateusz
 */

#ifndef SRC_LOGDUMPERTEXTFILE_H_
#define SRC_LOGDUMPERTEXTFILE_H_

#include <string>
#include "time.h"

#include "../shared/event_log_t.h"
#include "table.h"

class LogDumperTextFile {

	/**
	 * Internal object from ctable library representing a table all event log
	 * records will be printed in (presumably in some pretty way :) )
	 */
	struct Table * table;

	std::string fn;

	bool lineAbove = false;

	/**
	 * Stores all Timesync events with dedicatet format
	 * @param eventLogEntry
	 * @param timestamp
	 */
	void storeTimesyncEntryInExport(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp);

	void storeHardfaultException(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp);
	void storeSupervisorFault(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp);
	void storeSupervisorFaultTimestamp(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp);

	void storeCyclic(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp);
	void storeBootupComplete(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp);

	void storeGsmRegisteredNetwork(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp);
	void storeGsmImsi(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp);
	void storeGsmIpAddress(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp);

	void storeFanetFail(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp);


public:
	LogDumperTextFile();
	virtual ~LogDumperTextFile();

	void startTextExport(std::string filename);


	void storeEntryInExport(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp);
	void closeAndSaveTextExport(void);
};

#endif /* SRC_LOGDUMPERTEXTFILE_H_ */
