/*
 * LogDumper.h
 *
 *  Created on: Nov 8, 2024
 *      Author: mateusz
 */

#ifndef SRC_LOGDUMPER_H_
#define SRC_LOGDUMPER_H_

#include <string>
#include <atomic>

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

	std::atomic<bool> timeout;

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

	/**
	 * Reads the event log from the controller's flash one entry at a time, then writes
	 * a raw binary dump and a decoded text report.
	 *
	 * The address range [startAddress, endAddress) holds N amount of events, each sizeof(event_log_t).
	 * This are by definition is defined not to leave any unused tail, so it's size is
	 * divisible by sizeof(event_log_t). The function sends a read memory request through 
	 * SrvReadMemory and waits for the response. After a timeout, the same entry
	 * is requested again. There is no retry limit, so the function blocks until the
	 * controller answers.
	 *
	 * Each response is appended unchanged to "<filename>.bin", whether or not its CRC is
	 * correct. Its CRC is then checked and the entry decoded by convertEventToExposedEvent().
	 * Entries with a correct CRC go into the text report.
	 *
	 * @param startAddress			first address of the event log area in controller memory
	 * @param endAddress			end of the event log area (exclusive)
	 * @param filename				base name of the output files; the text report is created
	 * 								by LogDumperTextFile, the raw dump is saved as filename + ".bin"
	 * @param exitOnFirstCrcFail	if true, stop at the first entry with a wrong CRC; files
	 * 								are still closed and saved with what has been read so far
	 * @param logOldestEntryAddress	address of the oldest log entry
	 * @param logNewestEntryAddress	address of the newest log entry
	 * @param lastNewestEvents		if set to non-zero this function will download N newest 
	 * 								events. If the value is bigger than amount of events
	 * 								from [startAddress, logNewestEntryAddress) no roll over 
	 * 								will be performed
	 */
	void dumpEventsToReport (uint32_t startAddress, uint32_t endAddress, std::string filename,
							 bool exitOnFirstCrcFail, uint32_t logOldestEntryAddress,
							 uint32_t logNewestEntryAddress, size_t lastNewestEvents);
};

#endif /* SRC_LOGDUMPER_H_ */
