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

public:
	LogDumperTextFile();
	virtual ~LogDumperTextFile();

	void startTextExport(std::string filename);


	void storeEntryInExport(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp);
	void closeAndSaveTextExport(void);
};

#endif /* SRC_LOGDUMPERTEXTFILE_H_ */
