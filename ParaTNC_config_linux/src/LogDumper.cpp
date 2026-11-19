/*
 * LogDumper.cpp
 *
 *  Created on: Nov 8, 2024
 *      Author: mateusz
 */

#include <LogDumper.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <stdexcept>

#include "pthread.h"

#include "../shared/crc//crc_.h"

const uint16_t LogDumper::daysInYearByMonth[2][13] = {
	/* Normal years.  */
	{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
	/* Leap years.  */
	{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}};

LogDumper::LogDumper (SrvReadMemory &_srvReadMemory, sem_t &_cond1,
					  SerialRxBackgroundWorker &_serial_thread,
					  std::unique_ptr<EventDescriptionsStorage> &&_jsonEvents, bool decodeJsonOnly)
	: srvReadMemory (_srvReadMemory), cond1 (_cond1), serialRxBackgroundWorker (_serial_thread),
	  jsonEvents (std::move (_jsonEvents)), decodeFromJsonOnly (decodeJsonOnly)
{
	// TODO Auto-generated constructor stub
	timeout = false;
	progress = 0;
}

LogDumper::~LogDumper ()
{
	// TODO Auto-generated destructor stub
}

void LogDumper::getTimestampFromEvent (const event_log_exposed_t *const in, struct tm *out)
{

	uint16_t minutes_from_midnight = in->event_rtc & 0x7FFu;
	uint16_t days_from_new_year = (uint16_t)((uint32_t)(in->event_rtc >> 16) & 0x1FFu);
	uint8_t years = (uint8_t)((in->event_rtc >> 25) & 0x7Fu);

	out->tm_hour = minutes_from_midnight / 60;
	out->tm_min = minutes_from_midnight - (out->tm_hour * 60);
	out->tm_sec = 0;

	out->tm_year = years + 100;
	const uint16_t realYear = years + 2000;
	const bool isLeap = ((realYear % 4) == 0) ? true : false;

	int month = 0;
	int dayOfMonth = 0;

	if (days_from_new_year >= 1 && days_from_new_year <= 366) {
		for (int mon = 0; mon < 12; mon++) {
			if (days_from_new_year <= daysInYearByMonth[(isLeap) ? 1 : 0][mon + 1]) {
				month = mon + 1;
				dayOfMonth = days_from_new_year - daysInYearByMonth[(isLeap) ? 1 : 0][mon];
				break;
			}
		}
		out->tm_mon = month - 1;
		out->tm_mday = dayOfMonth;
	}
}

bool LogDumper::convertEventToExposedEvent (const event_log_t *event, event_log_exposed_t &exposed,
											struct tm &decodedDateTime)
{
	bool crcCorrect = false;

	if ((event->event_counter_id != 0) && (event->event_counter_id != 0xFFFFFFFF)) {

		// calculate crc checksum for this entry
		const uint32_t crc = calcCRC32std ((void *)(event),
										   sizeof (event_log_t) - 1,
										   0x04C11DB7,
										   0xFFFFFFFF,
										   0,
										   0,
										   0);

		const uint8_t crcLsb = (uint8_t)(crc & 0x000000FF);

		if (event->crc_checksum == crcLsb) {

			exposed.event_counter_id = event->event_counter_id;
			exposed.event_rtc = event->event_rtc;
			exposed.event_master_time = event->event_master_time;

			exposed.source = (event_log_source_t)EVENT_LOG_GET_SOURCE (event->source);
			exposed.source_str_name = event_log_source_to_str (exposed.source);

			exposed.severity = (event_log_severity_t)EVENT_LOG_GET_SEVERITY (event->severity);
			exposed.severity_str = event_log_severity_to_str (exposed.severity);

			exposed.event_id = event->event_id;
			exposed.event_str_name = event_id_to_str (exposed.source, exposed.event_id);

			exposed.lparam = event->lparam;
			exposed.lparam2 = event->lparam2;
			exposed.wparam = event->wparam;
			exposed.wparam2 = event->wparam2;
			exposed.param = event->param;
			exposed.param2 = event->param2;

			if (event->event_rtc != 0x0) {
				LogDumper::getTimestampFromEvent (&exposed, &decodedDateTime);
			}

			crcCorrect = true;
		}
		else {
			std::cout << "E = LogDumper::convertEventToExposedEvent, crc error, expected 0x"
					  << std::hex << (int)event->crc_checksum << ", got: 0x" << (int)crcLsb
					  << std::endl;
		}
	}
	else {
		std::cout
			<< "W = LogDumper::convertEventToExposedEvent, probably erased memory has been read."
			<< std::endl;
	}

	return crcCorrect;
}

void LogDumper::timeoutCallback (void)
{
	timeout = true;
	sem_post (&cond1);
}

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
void LogDumper::dumpEventsToReport (uint32_t startAddress, uint32_t endAddress,
									std::string filename, bool exitOnFirstCrcFail,
									uint32_t logOldestEntryAddress, uint32_t logNewestEntryAddress,
									size_t lastNewestEvents)
{

	uint32_t decoded_events = 0u;
	uint32_t events_with_failed_crc = 0u;

	bool hasDecodedEvents = false;

	struct tm decodedDateTime = {0u};

	event_log_exposed_t exposed = {0u};

	// keep for sanity check
	const uint32_t originalStartAddress = startAddress;

	// how many events to dump
	int eventsNum;

	// declaring mutex
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	const size_t logEntrySize = sizeof (event_log_t);

	// in events
	const size_t logAreaSize = (endAddress - startAddress) / logEntrySize;

	if (lastNewestEvents > logAreaSize) {
		throw std::runtime_error (
			"You manually selected to dump more events that a size of area in the controller");
	}

	if (logOldestEntryAddress < startAddress || logOldestEntryAddress > endAddress) {
		throw std::runtime_error("Corrupted value of 'logOldestEntryAddress' - application cannot continue");
	}

	if (logNewestEntryAddress < startAddress || logNewestEntryAddress > endAddress) {
		throw std::runtime_error("Corrupted value of 'logOldestEntryAddress' - application cannot continue");
	}

	// check if all events shall be dumped or only 'lastNewestEvents'
	if (lastNewestEvents != 0) {
		// size in bytes
		const size_t sizeOfEventsToDmp = logEntrySize * lastNewestEvents;

		// value of logNewestEntryAddress points to the start of an event
		const uint32_t endAddressOfNewest = logNewestEntryAddress + logEntrySize;

		// check if we fit within log entry area
		if (endAddressOfNewest - sizeOfEventsToDmp >= startAddress) {
			// we fit, so update start and end address
			startAddress = endAddressOfNewest - sizeOfEventsToDmp;
			endAddress = endAddressOfNewest;
		}
		else {
			// here we want to dump less events, because we must trim the memory range
			lastNewestEvents = (endAddressOfNewest - startAddress) / logEntrySize;

			// update end address, but keep start unchanged
			endAddress = logNewestEntryAddress;

		}

		eventsNum = lastNewestEvents;
	}
	else {
		eventsNum = (endAddress - startAddress) / logEntrySize;
	}

	// log dumper shall never wrap around. If 'lastNewestEvents' is bigger than
	// amount of events from startAddress until, the range shall be trimmed 
	// starting from 'originalStartAddress'
	if (endAddress < startAddress) {
		throw std::runtime_error("safety check 'endAddress < startAddress' has failed");
	}

	if (startAddress < originalStartAddress) {
		throw std::runtime_error("safety check 'startAddress < originalStartAddress' has failed");
	}

	// check if dumper should use JSON database
	if (jsonEvents) {
		hasDecodedEvents = true;
	}
	else {
		hasDecodedEvents = false;
	}

	// stream to store RAW binary read from the controller
	std::ofstream outBin (filename + ".bin", std::ios::out | std::ios::binary);

	std::cout << "I = LogDumper::dumpEventsToReport, number of events to process: " << std::dec
			  << eventsNum << std::endl;

	// this initializes ctable library. it does not create a file on disk
	logDumperTextFile.startTextExport (filename);

	// create a binding the timeout callback, which will simply push the process
	// further (to the same event). binding in a context of current instance
	auto timeoutCallback = std::bind (&LogDumper::timeoutCallback, this);

	for (int i = 0; i < eventsNum; i++) {
		serialRxBackgroundWorker.backgroundTimeoutCallback = timeoutCallback;

		const int currentProgress = (int)(((float)i / (float)eventsNum) * 100.0f);

		std::cout << std::dec << "I = LogDumper::dumpEventsToReport, processing event " << i
				  << ". Total progress: " << currentProgress << "%" << std::endl;
		const uint32_t address = startAddress + (logEntrySize * i);
		srvReadMemory.sendRequestForMemoryRange (address, sizeof (event_log_t));

		pthread_mutex_lock (&lock);
		// wait for configuration to be received
		sem_wait (&cond1);
		pthread_mutex_unlock (&lock);

		if (timeout) {
			// repeat the same event once again
			i--;
			timeout = false;
		}
		else {

			std::vector<uint8_t> memoryData = srvReadMemory.getResponseData ();

			// append raw binary data to the output file
			if (outBin && outBin.good ()) {
				std::ostream_iterator<char> oi (outBin);
				std::copy (memoryData.begin (), memoryData.end (), oi);
			}

			const event_log_t *event = (event_log_t *)memoryData.data ();

			if (memoryData.size () < logEntrySize) {
				std::cout << "E = LogDumper::dumpEventsToReport, data read from address 0x"
						  << std::hex << address << " is too small to be correct event_log_t"
						  << std::endl;
				continue;
			}

			// check if an event is not corrupted (by CRC32 calculation) and decode it
			const bool crc =
				LogDumper::convertEventToExposedEvent (event, exposed, decodedDateTime);

			// if crc for decoded event is correct
			if (crc) {
				decoded_events++;

				// if JSON file with descriptions should be used
				// user provided 'decode-json' or 'decode-json-only'
				if (hasDecodedEvents) {
					// look for a description for current event.
					// it is guaranteed at this point, that a pointer is set to something
					std::optional<EventDescriptionObject> description =
						jsonEvents->getForEvent (*event);

					// if a description for this event has been found
					if (description) {
						EventDescriptionObject &descriptionObject = description.value ();
						logDumperTextFile.storeEntryInExport (&exposed,
															  &decodedDateTime,
															  &descriptionObject,
															  decodeFromJsonOnly);
					}
				}
				else {
					// if not simply store this in output file using some hardcoded decoding
					// for specific events
					logDumperTextFile.storeEntryInExport (&exposed, &decodedDateTime);
				}
			}
			else {
				events_with_failed_crc++;
				std::cout << "E = LogDumper::dumpEventsToReport, crc error for address 0x"
						  << std::hex << address << std::endl;

				if (exitOnFirstCrcFail) {
					break;
				}
			}
		}

		if (progress != currentProgress) {
			// logDumperTextFile.closeAndSaveTextExport();
		}

		progress = currentProgress;
	}

	if (outBin && outBin.good ()) {
		outBin.close ();
	}

	logDumperTextFile.closeAndSaveTextExport ();

	std::cout << "I = LogDumper::dumpEventsToReport, decoded event log entries: " << std::dec
			  << decoded_events << std::endl;
	std::cout << "I = LogDumper::dumpEventsToReport, events with CRC checksum error: " << std::dec
			  << events_with_failed_crc << std::endl;
}
