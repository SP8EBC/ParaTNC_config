/*
 * LogDumperTextFile.cpp
 *
 *  Created on: Nov 13, 2024
 *      Author: mateusz
 */

#include <LogDumperTextFile.h>
#include <string.h>
#include <iostream>

#include "../shared/event_log_strings.h"

LogDumperTextFile::LogDumperTextFile() {
	// TODO Auto-generated constructor stub
	table = NULL;
}

LogDumperTextFile::~LogDumperTextFile() {
	// TODO Auto-generated destructor stub
}

void LogDumperTextFile::startTextExport(std::string filename) {
	// 13 columns
	::TableHAlign hAlign[13];
	::TableVAlign vAlign[13];

	memset(hAlign, H_ALIGN_LEFT, 13 * sizeof(::TableHAlign));
	memset(vAlign, V_ALIGN_CENTER, 13 * sizeof(::TableVAlign));

	table = get_empty_table();

	set_default_alignments(table, 13, hAlign, vAlign);

	add_empty_cell(table);
	add_empty_cell(table);
	//add_cell(table, "Counter");
	add_cell(table, "RTC timestamp");
	add_cell(table, "Uptime [s]");
	add_cell(table, "Source");
	add_cell(table, "Event");
	add_cell(table, "param");
	add_cell(table, "param2");
	add_cell(table, "wparam");
	add_cell(table, "wparam2");
	add_cell(table, "wparam3");
	add_cell(table, "lparam");
	add_cell(table, "lparam2");
	next_row(table);
//	set_span(table, 13, 1);
//	add_cell(table, "test");

	fn = filename;

}

void LogDumperTextFile::storeEntryInExport(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp) {

	// special cases
	auto src = eventLogEntry->source;
	auto svrty = eventLogEntry->severity;
	auto id = eventLogEntry->event_id;

	if (src == EVENT_SRC_MAIN && svrty == EVENT_TIMESYNC && id == EVENTS_MAIN_TIMESYNC_NTP) {
		storeTimesyncEntryInExport(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_MAIN && svrty == EVENT_ERROR && id == EVENTS_MAIN_POSTMORTEM_HARDFAULT) {
		storeHardfaultException(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_MAIN && svrty == EVENT_ERROR && id == EVENTS_MAIN_POSTMORTEM_SUPERVISOR) {
		storeSupervisorFault(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_MAIN && svrty == EVENT_INFO_CYCLIC && id == EVENTS_MAIN_CYCLIC) {
		storeCyclic(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_MAIN && svrty == EVENT_ERROR && id == EVENTS_MAIN_CONFIG_FIRST_CRC_FAIL) {
		storeConfigFirstCrcFail(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_MAIN && svrty == EVENT_ERROR && id == EVENTS_MAIN_CONFIG_SECOND_CRC_FAIL) {
		storeConfigSecondCrcFail(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_MAIN && id == EVENTS_MAIN_CONFIG_FIRST_RESTORE) {
		storeConfigFirstRestore(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_MAIN && id == EVENTS_MAIN_CONFIG_SECOND_RESTORE) {
		storeConfigSecondRestore(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_GSM_GPRS && svrty == EVENT_BOOTUP && id == EVENTS_GSM_GPRS_REGISTERED_NETWORK) {
		storeGsmRegisteredNetwork(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_GSM_GPRS && svrty == EVENT_BOOTUP && id == EVENTS_GSM_GPRS_IMSI) {
		storeGsmImsi(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_GSM_GPRS && svrty == EVENT_BOOTUP && id == EVENTS_GSM_GPRS_IP_ADDRESS) {
		storeGsmIpAddress(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_MAIN && svrty == EVENT_BOOTUP && id == EVENTS_MAIN_BOOTUP_COMPLETE) {
		storeBootupComplete(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_FANET && svrty == EVENT_ERROR && id == EVENTS_FANET_FAIL_TO_SEND_METEO) {
		storeFanetFail(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_KISS && id == EVENTS_DEFINITIONS_KISS_WARN_ERASING_STARTUP) {
		storeErasingStartup(eventLogEntry,timestamp);
	}
	else if (src == EVENT_SRC_KISS && id == EVENTS_DEFINITIONS_KISS_WARN_FLASHING_STARTUP) {
		storeFlashingStartup(eventLogEntry,timestamp);
	}
	else {

	// add_cell_fmt
		add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
		add_cell_fmt(table, "%s", eventLogEntry->severity_str);
		add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
		add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
		add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
		add_cell_fmt(table, "%s", eventLogEntry->event_str_name);
		add_cell_fmt(table, "0x%X", (uint32_t)eventLogEntry->param);
		add_cell_fmt(table, "0x%X", (uint32_t)eventLogEntry->param2);
		add_cell_fmt(table, "0x%X", (uint32_t)eventLogEntry->wparam);
		add_cell_fmt(table, "0x%X", (uint32_t)eventLogEntry->wparam2);
		add_cell_fmt(table, "0x%X", (uint32_t)eventLogEntry->wparam3);
		add_cell_fmt(table, "0x%X", (uint32_t)eventLogEntry->lparam);
		add_cell_fmt(table, "0x%X", (uint32_t)eventLogEntry->lparam2);
		next_row(table);

	}

}

void LogDumperTextFile::storeHardfaultException(
		const event_log_exposed_t *eventLogEntry,
		const struct tm *const timestamp)
{
	char * first_register = "   ";
	char * second_register = "   ";

	switch (eventLogEntry->param)
	{
	case 1: first_register = (char *)event_log_str_hardfault_lr; second_register = (char *)event_log_str_hardfault_pc; break;
	case 2: first_register = (char *)event_log_str_hardfault_r0; second_register = (char *)event_log_str_hardfault_r1; break;
	case 3: first_register = (char *)event_log_str_hardfault_r2; second_register = (char *)event_log_str_hardfault_r3; break;
	case 4: first_register = (char *)event_log_str_hardfault_r12; second_register = (char *)event_log_str_hardfault_cfsr; break;
	case 5: first_register = (char *)event_log_str_hardfault_src; second_register = (char *)event_log_str_hardfault_xpsr; break;
	default:
		{
			std::cout << "E = LogDumperTextFile::storeHardfaultException, unknown value of 'param': 0x" << std::hex << (int)eventLogEntry->param <<std::endl;
			std::cout << "E = LogDumperTextFile::storeHardfaultException, lparam: 0x" << std::hex << eventLogEntry->lparam <<std::endl;
			std::cout << "E = LogDumperTextFile::storeHardfaultException, lparam2: 0x" << std::hex << eventLogEntry->lparam2 <<std::endl;

			break;
		}
	}

    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "%s", eventLogEntry->event_str_name);
	set_span(table, 3, 1);
	//override_above_border(table, BORDER_SINGLE);
	add_cell_fmt(table, "%s: 0x%lX", first_register, eventLogEntry->lparam);
	set_span(table, 4, 1);
	add_cell_fmt(table, "%s: 0x%lX", second_register, eventLogEntry->lparam2);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::storeSupervisorFault(
		const event_log_exposed_t *eventLogEntry,
		const struct tm *const timestamp)
{
	// task name stored in params is 10 char max, but make a room for null terminator
	char taskName[11];
	memset(taskName, 0x00, 11);

	if (eventLogEntry->param == 0xFFu)
	{
		if (eventLogEntry->param2 == 0xFFu)
		{
			// this is timestamp
			storeSupervisorFaultTimestamp(eventLogEntry, timestamp);
			return;
		}
	}

	taskName[0] = eventLogEntry->param;
	taskName[1] = eventLogEntry->param2;
	taskName[2] = (eventLogEntry->wparam) & 0xFFu;
	taskName[3] = (eventLogEntry->wparam >> 8u) & 0xFFu;
	taskName[4] = (eventLogEntry->wparam2) & 0xFFu;
	taskName[5] = (eventLogEntry->wparam2 >> 8u) & 0xFFu;
	taskName[6] = (eventLogEntry->lparam) & 0xFFu;
	taskName[7] = (eventLogEntry->lparam >> 8u) & 0xFFu;
	taskName[8] = (eventLogEntry->lparam >> 16u) & 0xFFu;
	taskName[9] = (eventLogEntry->lparam >> 24u) & 0xFFu;

	const uint16_t scaled_time = eventLogEntry->wparam3;
	const uint32_t checkpoints = eventLogEntry->lparam2;

	bool thisFailed = ((checkpoints & 0x80000000u) != 0);

    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "Supervisor fail");
	set_span(table, 7, 1);
	//override_above_border(table, BORDER_SINGLE);
	if (thisFailed)
	{
		add_cell_fmt(table, "%s: last supervisor %d msec ago, checkpoints: 0x%X, THIS FAILED",
				taskName, scaled_time * 20, checkpoints & 0x7FFFFFFFu);
	}
	else
	{
		add_cell_fmt(table, "%s: last supervisor %d msec ago, checkpoints: 0x%X",
				taskName, scaled_time * 20, checkpoints & 0x7FFFFFFFu);
	}

	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::storeSupervisorFaultTimestamp(
		const event_log_exposed_t *eventLogEntry,
		const struct tm *const timestamp)
{

    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "Supervisor fail");
	set_span(table, 7, 1);
	//override_above_border(table, BORDER_SINGLE);
		add_cell_fmt(table, "maser timestamp at which supervisor failed: %d",
				eventLogEntry->lparam);

	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::storeTimesyncEntryInExport(
		const event_log_exposed_t *eventLogEntry,
		const struct tm *const timestamp)
{
    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "RTC synchronized from Network Time Srvr");
	set_span(table, 7, 1);
	//override_above_border(table, BORDER_SINGLE);
	add_cell_fmt(table, "Real time clock set to %02d-%02d-%02d %02d:%02d",
						eventLogEntry->param,
						eventLogEntry->param2,
						eventLogEntry->wparam,
						eventLogEntry->wparam2,
						eventLogEntry->lparam,
						eventLogEntry->lparam2);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::storeCyclic(const event_log_exposed_t *eventLogEntry,
		const struct tm *const timestamp)
{
    set_hline(table, BORDER_SINGLE);
	set_span(table, 1, 2);
	override_vertical_alignment(table, V_ALIGN_CENTER);
	//add_cell(table, "dupa");
	add_cell_fmt(table, "%d \n -", eventLogEntry->event_counter_id);
	set_span(table, 1, 2);
	override_vertical_alignment(table, V_ALIGN_CENTER);
	add_cell_fmt(table, "%s \n -", eventLogEntry->severity_str);
	set_span(table, 1, 2);
	override_vertical_alignment(table, V_ALIGN_CENTER);
	add_cell_fmt(table, "%02d-%02d %02d:%02d \n -", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	set_span(table, 1, 2);
	override_vertical_alignment(table, V_ALIGN_CENTER);
	add_cell_fmt(table, "%d \n -", eventLogEntry->event_master_time / 1000u);
	set_span(table, 1, 2);
	override_vertical_alignment(table, V_ALIGN_CENTER);
	add_cell_fmt(table, "%s \n -", eventLogEntry->source_str_name);
	set_span(table, 1, 2);
	override_vertical_alignment(table, V_ALIGN_CENTER);
	add_cell_fmt(table, "Cyclic status report\n -");
	set_span(table, 7, 1);
	add_cell_fmt(table, "APRS-IS stats, connections: %u, conn fails: %u, packets tx: %u",
						eventLogEntry->param,
						eventLogEntry->param2,
						eventLogEntry->wparam);
	next_row(table);
	set_span(table, 7, 1);
	add_cell_fmt(table, "Battery: %4.2f V, total RX: %d pkts, total TX: %d pkts",
						((float)eventLogEntry->wparam2 / 100.0f),
						eventLogEntry->lparam,
						eventLogEntry->lparam2);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
}

void LogDumperTextFile::storeBootupComplete(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp)
{
    set_hline(table, BORDER_SINGLE);
	set_span(table, 1, 2);
	override_vertical_alignment(table, V_ALIGN_CENTER);
	//add_cell(table, "dupa");
	add_cell_fmt(table, "%d \n -", eventLogEntry->event_counter_id);
	set_span(table, 1, 2);
	override_vertical_alignment(table, V_ALIGN_CENTER);
	add_cell_fmt(table, "%s \n -", eventLogEntry->severity_str);
	set_span(table, 1, 2);
	override_vertical_alignment(table, V_ALIGN_CENTER);
	add_cell_fmt(table, "%02d-%02d %02d:%02d \n -", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	set_span(table, 1, 2);
	override_vertical_alignment(table, V_ALIGN_CENTER);
	add_cell_fmt(table, "%d \n -", eventLogEntry->event_master_time / 1000u);
	set_span(table, 1, 2);
	override_vertical_alignment(table, V_ALIGN_CENTER);
	add_cell_fmt(table, "%s \n -", eventLogEntry->source_str_name);
	set_span(table, 1, 2);
	override_vertical_alignment(table, V_ALIGN_CENTER);
	add_cell_fmt(table, "Bootup complete\n -");
	set_span(table, 7, 1);
	add_cell_fmt(table, "RTC ok: %u, Last powersave st: %u, : Battery: %4.2f V",
						eventLogEntry->param,
						eventLogEntry->param2,
						((float)eventLogEntry->wparam / 100.0f));
	next_row(table);
	set_span(table, 7, 1);
	add_cell_fmt(table, "monitor: 0x%X, go-to-sleep events cntr: %u",
						eventLogEntry->lparam,
						eventLogEntry->lparam2);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
}

void LogDumperTextFile::storeGsmRegisteredNetwork(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp)
{
	char carrierName[16] = {0u};

	memcpy((void*)&carrierName[12], (void*)&eventLogEntry->wparam, sizeof(uint8_t));
	memcpy((void*)&carrierName[13], (void*)&eventLogEntry->wparam2, sizeof(uint8_t));
	memcpy((void*)&carrierName[8], (void*)&eventLogEntry->wparam, sizeof(uint16_t));
	memcpy((void*)&carrierName[10], (void*)&eventLogEntry->wparam2, sizeof(uint16_t));
	memcpy((void*)&carrierName[0], (void*)&eventLogEntry->lparam, sizeof(uint32_t));
	memcpy((void*)&carrierName[4], (void*)&eventLogEntry->lparam2, sizeof(uint32_t));

    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "GSM network name");
	set_span(table, 7, 1);
	//override_above_border(table, BORDER_SINGLE);
	add_cell_fmt(table, " %s", carrierName);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::storeGsmImsi(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp)
{
	char imsi[16] = {0u};

	memcpy((void*)&imsi[12], (void*)&eventLogEntry->wparam, sizeof(uint8_t));
	memcpy((void*)&imsi[13], (void*)&eventLogEntry->wparam2, sizeof(uint8_t));
	memcpy((void*)&imsi[8], (void*)&eventLogEntry->wparam, sizeof(uint16_t));
	memcpy((void*)&imsi[10], (void*)&eventLogEntry->wparam2, sizeof(uint16_t));
	memcpy((void*)&imsi[0], (void*)&eventLogEntry->lparam, sizeof(uint32_t));
	memcpy((void*)&imsi[4], (void*)&eventLogEntry->lparam2, sizeof(uint32_t));

    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "GSM IMSI");
	set_span(table, 7, 1);
	add_cell_fmt(table, " %s", imsi);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::storeFanetFail(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp)
{
    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "%s", eventLogEntry->event_str_name);

	const uint8_t fanet_fail_counter = eventLogEntry->param;
	const uint8_t fanet_success_counter = eventLogEntry->param2;

	const int32_t errorCode = (int32_t)eventLogEntry->lparam2;

	set_span(table, 7, 1);
	add_cell_fmt(table, "Return code: %d, succeeded %d x, failed %d x",
						errorCode, fanet_success_counter, fanet_fail_counter);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::storeGsmIpAddress(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp)
{
	char ipAddr[16] = {0u};

	memcpy((void*)&ipAddr[12], (void*)&eventLogEntry->wparam, sizeof(uint8_t));
	memcpy((void*)&ipAddr[13], (void*)&eventLogEntry->wparam2, sizeof(uint8_t));
	memcpy((void*)&ipAddr[8], (void*)&eventLogEntry->wparam, sizeof(uint16_t));
	memcpy((void*)&ipAddr[10], (void*)&eventLogEntry->wparam2, sizeof(uint16_t));
	memcpy((void*)&ipAddr[0], (void*)&eventLogEntry->lparam, sizeof(uint32_t));
	memcpy((void*)&ipAddr[4], (void*)&eventLogEntry->lparam2, sizeof(uint32_t));

    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "GPRS Ip Address");
	set_span(table, 7, 1);
	add_cell_fmt(table, " %s", ipAddr);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::closeAndSaveTextExport(void) {
    set_all_vlines(table, BORDER_SINGLE);
	next_row(table);
    make_boxed(table, BORDER_DOUBLE);

    FILE* output;
    output = fopen(fn.c_str(), "w");

    fprint_table(table, output);

    fclose(output);
}

void LogDumperTextFile::storeErasingStartup (const event_log_exposed_t *eventLogEntry,
											 const struct tm *const timestamp)
{
    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "%s", eventLogEntry->event_str_name);
	set_span(table, 7, 1);
	add_cell_fmt(table, "result: %d", eventLogEntry->param);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::storeFlashingStartup (const event_log_exposed_t *eventLogEntry,
											  const struct tm *const timestamp)
{
    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "%s", eventLogEntry->event_str_name);
	set_span(table, 7, 1);
	add_cell_fmt(table, "result: %d", eventLogEntry->param);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::storeConfigFirstCrcFail (const event_log_exposed_t *eventLogEntry,
												 const struct tm *const timestamp)
{
    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "%s", eventLogEntry->event_str_name);
	set_span(table, 7, 1);
	add_cell_fmt(table, "expected: 0x%X, calculated: 0x%X", eventLogEntry->lparam, eventLogEntry->lparam2);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::storeConfigSecondCrcFail (const event_log_exposed_t *eventLogEntry,
												  const struct tm *const timestamp)
{
    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "%s", eventLogEntry->event_str_name);
	set_span(table, 7, 1);
	add_cell_fmt(table, "expected: 0x%X, calculated: 0x%X", eventLogEntry->lparam, eventLogEntry->lparam2);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::storeConfigFirstRestore (const event_log_exposed_t *eventLogEntry,
												 const struct tm *const timestamp)
{
    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "%s", eventLogEntry->event_str_name);
	set_span(table, 7, 1);
	add_cell_fmt(table, "param: d, target CRC: 0x%X", eventLogEntry->param, eventLogEntry->lparam);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}

void LogDumperTextFile::storeConfigSecondRestore (const event_log_exposed_t *eventLogEntry,
												  const struct tm *const timestamp)
{
    set_hline(table, BORDER_SINGLE);
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%s", eventLogEntry->severity_str);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_master_time / 1000u);
	add_cell_fmt(table, "%s", eventLogEntry->source_str_name);
	add_cell_fmt(table, "%s", eventLogEntry->event_str_name);
	set_span(table, 7, 1);
	add_cell_fmt(table, "param: d, target CRC: 0x%X", eventLogEntry->param, eventLogEntry->lparam);
	next_row(table);
    set_hline(table, BORDER_SINGLE);
	lineAbove = true;
}
