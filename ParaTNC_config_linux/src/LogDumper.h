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

	/**
	 * Class writing report
	 */
	LogDumperTextFile logDumperTextFile;

	static void getTimestampFromEvent(const event_log_exposed_t * const in, struct tm * out);

	static const uint16_t daysInYearByMonth[2][13];
//											   =
//	{
//	    /* Normal years.  */
//	    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
//	    /* Leap years.  */
//	    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
//	};

public:

	LogDumper(SrvReadMemory& _srvReadMemory, pthread_cond_t& _cond1);
	virtual ~LogDumper();

	void dumpEventsToReport(uint32_t startAddress, uint32_t endAddress, std::string filename);
};

#endif /* SRC_LOGDUMPER_H_ */
