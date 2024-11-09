/*
 * LogDumper.cpp
 *
 *  Created on: Nov 8, 2024
 *      Author: mateusz
 */

#include <LogDumper.h>

#include "../shared/event_log.h"
#include "../shared/crc//crc_.h"


#include <iostream>

LogDumper::LogDumper(SrvReadMemory& _srvReadMemory, pthread_cond_t& _cond1) : srvReadMemory(_srvReadMemory), cond1(_cond1) {
	// TODO Auto-generated constructor stub

}

LogDumper::~LogDumper() {
	// TODO Auto-generated destructor stub
}

void LogDumper::dumpEventsToReport(uint32_t startAddress, uint32_t endAddress,
		std::string filename) {

	uint32_t decoded_events = 0u;

	// declaring mutex
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	const size_t logEntrySize = sizeof(event_log_t);

	const int eventsNum = (endAddress - startAddress) / logEntrySize;

	for (int i = 0; i < eventsNum; i++)
	{
	    const uint32_t address = startAddress + (logEntrySize * i);
	    srvReadMemory.sendRequestForMemoryRange(address, sizeof(event_log_t));
	    srvReadMemory.waitForTransmissionDone();

	    pthread_mutex_lock(&lock);
	    // wait for configuration to be received
	    pthread_cond_wait(&cond1, &lock);
	    pthread_mutex_unlock(&lock);

	    std::vector<uint8_t> memoryData = srvReadMemory.getResponseData();

		event_log_t * event = (event_log_t *)memoryData.data();

		if ((event->event_counter_id != 0) && (event->event_counter_id != 0xFFFFFFFF)) {

			// backup event counter value
			const uint32_t eventCounter = event->event_counter_id;

			// clear this prior to CRC calculation
			event->event_counter_id = 0;

			// calculate crc checksum for this entry
			const uint32_t crc = calcCRC32std((void*)(&memoryData[0]), logEntrySize - 1, 0x04C11DB7, 0xFFFFFFFF, 0, 0, 0);

			const uint8_t crcLsb = (uint8_t)(crc & 0x000000FF);

			// restore event counter value
			event->event_counter_id = eventCounter;

			if (event->crc_checksum == crcLsb) {
				decoded_events++;
			}
			else {
				std::cout << "E = LogDumper::dumpEventsToReport, crc error for address 0x" << std::hex << address <<std::endl;
				std::cout << "E = LogDumper::dumpEventsToReport, expected 0x" << std::hex << (int)event->crc_checksum << ", got: 0x" << (int)crcLsb << std::endl;
			}

		}
	}

	std::cout << "I = LogDumper::dumpEventsToReport, decoded event log entries: " << std::dec << decoded_events << std::endl;
}

