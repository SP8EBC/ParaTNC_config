/*
 * LogDumperTextFile.cpp
 *
 *  Created on: Nov 13, 2024
 *      Author: mateusz
 */

#include <LogDumperTextFile.h>
#include <string.h>

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
// add_cell_fmt
	add_cell_fmt(table, "%d", eventLogEntry->event_counter_id);
	add_cell_fmt(table, "%02d-%02d %02d:%02d", timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_hour, timestamp->tm_min);
	add_cell_fmt(table, "%d", eventLogEntry->event_rtc / 1000u);
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

void LogDumperTextFile::closeAndSaveTextExport(void) {
    set_all_vlines(table, BORDER_SINGLE);
	next_row(table);
    make_boxed(table, BORDER_DOUBLE);

    FILE* output;
    output = fopen(fn.c_str(), "w");

    fprint_table(table, output);

    fclose(output);

}
