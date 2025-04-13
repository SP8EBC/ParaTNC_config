/*
 * LogDumper.cpp
 *
 *  Created on: Nov 8, 2024
 *      Author: mateusz
 */

#include <LogDumper.h>
#include <iostream>
#include <utility>
#include <fstream>
#include <iterator>

#include "pthread.h"

#include "../shared/crc//crc_.h"

const uint16_t LogDumper::daysInYearByMonth[2][13] =
{
    /* Normal years.  */
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
    /* Leap years.  */
    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};


LogDumper::LogDumper(SrvReadMemory& _srvReadMemory, pthread_cond_t& _cond1, SerialRxBackgroundWorker& _serial_thread) :
								srvReadMemory(_srvReadMemory),
								cond1(_cond1),
								serialRxBackgroundWorker(_serial_thread){
	// TODO Auto-generated constructor stub
	timeout = false;
}

LogDumper::~LogDumper() {
	// TODO Auto-generated destructor stub
}

void LogDumper::getTimestampFromEvent(const event_log_exposed_t *const in,
		struct tm *out) {

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
           if (days_from_new_year <= daysInYearByMonth[(isLeap) ? 1 : 0][mon+1]) {
               month = mon + 1;
               dayOfMonth = days_from_new_year - daysInYearByMonth[(isLeap) ? 1 : 0][mon];
               break;
           }
        }
        out->tm_mon = month - 1;
        out->tm_mday = dayOfMonth;
    }

}

void LogDumper::timeoutCallback(void) {
	timeout = true;
	pthread_cond_signal(&cond1);
}

void LogDumper::dumpEventsToReport(uint32_t startAddress, uint32_t endAddress,
		std::string filename) {

	uint32_t decoded_events = 0u;

	struct tm decodedDateTime = {0u};

	// declaring mutex
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	std::ofstream outBin(filename + ".bin", std::ios::out | std::ios::binary);

	const size_t logEntrySize = sizeof(event_log_t);

	const int eventsNum = (endAddress - startAddress) / logEntrySize;

	std::cout << "I = LogDumper::dumpEventsToReport, number of events to process: " << std::dec << eventsNum << std::endl;

	logDumperTextFile.startTextExport(filename);

	auto timeoutCallback = std::bind(&LogDumper::timeoutCallback, this);

	for (int i = 0; i < eventsNum; i++)
	{
		serialRxBackgroundWorker.backgroundTimeoutCallback = timeoutCallback;

		std::cout << std::dec <<
				"I = LogDumper::dumpEventsToReport, processing event " << i <<
				". Total progress: " << (int)(((float)i / (float)eventsNum) * 100.0f) << "%" << std::endl;
	    const uint32_t address = startAddress + (logEntrySize * i);
	    srvReadMemory.sendRequestForMemoryRange(address, sizeof(event_log_t));
	    srvReadMemory.waitForTransmissionDone();

	    pthread_mutex_lock(&lock);
	    // wait for configuration to be received
	    pthread_cond_wait(&cond1, &lock);
	    pthread_mutex_unlock(&lock);

	    if (timeout) {
	    	// repeat the same event once again
	    	i--;
	    	timeout = false;
	    }
	    else {

			std::vector<uint8_t> memoryData = srvReadMemory.getResponseData();

			if (outBin && outBin.good()) {
			    std::ostream_iterator<char> oi(outBin);
			    std::copy(memoryData.begin(), memoryData.end(), oi);

				//outBin.write(memoryData.data(), memoryData.size());
			}

			const event_log_t * event = (event_log_t *)memoryData.data();

			if ((event->event_counter_id != 0) && (event->event_counter_id != 0xFFFFFFFF)) {

				// calculate crc checksum for this entry
				const uint32_t crc = calcCRC32std((void*)(&memoryData[0]), logEntrySize - 1, 0x04C11DB7, 0xFFFFFFFF, 0, 0, 0);

				const uint8_t crcLsb = (uint8_t)(crc & 0x000000FF);

				if (event->crc_checksum == crcLsb) {
					decoded_events++;

					event_log_exposed_t exposed = {0u};

					exposed.event_counter_id = event->event_counter_id;
					exposed.event_rtc = event->event_rtc;
					exposed.event_master_time = event->event_master_time;

					exposed.source = (event_log_source_t)EVENT_LOG_GET_SOURCE(event->source);
					exposed.source_str_name = event_log_source_to_str(exposed.source);

					exposed.severity = (event_log_severity_t)EVENT_LOG_GET_SEVERITY(event->severity);
					exposed.severity_str = event_log_severity_to_str(exposed.severity);

					exposed.event_id = event->event_id;
					exposed.event_str_name = event_id_to_str(exposed.source, exposed.event_id);

					exposed.lparam = event->lparam;
					exposed.lparam2 = event->lparam2;
					exposed.wparam = event->wparam;
					exposed.wparam2 = event->wparam2;
					exposed.param = event->param;
					exposed.param2 = event->param2;

					if (event->event_rtc != 0x0) {
						LogDumper::getTimestampFromEvent(&exposed, &decodedDateTime);
					}

					logDumperTextFile.storeEntryInExport(&exposed, &decodedDateTime);
				}
				else {
					std::cout << "E = LogDumper::dumpEventsToReport, crc error for address 0x" << std::hex << address <<std::endl;
					std::cout << "E = LogDumper::dumpEventsToReport, expected 0x" << std::hex << (int)event->crc_checksum << ", got: 0x" << (int)crcLsb << std::endl;
				}

			}
	    }

	}

	if (outBin && outBin.good()) {
		outBin.close();
	}

	logDumperTextFile.closeAndSaveTextExport();

	std::cout << "I = LogDumper::dumpEventsToReport, decoded event log entries: " << std::dec << decoded_events << std::endl;
}

