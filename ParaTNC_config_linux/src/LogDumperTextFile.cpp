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
	else if (src == EVENT_SRC_MAIN && svrty == EVENT_INFO_CYCLIC && id == EVENTS_MAIN_CYCLIC) {
		storeCyclic(eventLogEntry,timestamp);
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

void LogDumperTextFile::storeSupervisorException(
		const event_log_exposed_t *eventLogEntry,
		const struct tm *const timestamp)
{

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

void LogDumperTextFile::storeGsmRegisteredNetwork(const event_log_exposed_t * eventLogEntry, const struct tm * const timestamp) {

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
