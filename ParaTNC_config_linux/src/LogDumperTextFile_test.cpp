/*
 * LogDumper_test.cpp
 *
 *  Created on: Nov 8, 2024
 *      Author: mateusz
 */

#include <LogDumperTextFile.h>
#include <LogDumper.h>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LOGDUMPER
#include <boost/test/unit_test.hpp>


#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/logdumpertex_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_successful_tests);

  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

class _TestDumper : LogDumper {

public:
	static bool _convertEventToExposedEvent(const event_log_t * event, event_log_exposed_t & exposed, struct tm & decodedDateTime) {
		return LogDumper::convertEventToExposedEvent(event, exposed, decodedDateTime);
	}
};

BOOST_GLOBAL_FIXTURE (MyConfig);

BOOST_AUTO_TEST_CASE(read_binary_1) {
	LogDumperTextFile textFile;

	uint8_t entry[sizeof(event_log_t)];
	const event_log_t * event = (event_log_t *)entry;

	int crcErrors = 0;

    std::ifstream input( "./test_input/test__SR8WXD__2025-Apr-15_20-18-33.log.bin", std::ios::binary );

    event_log_exposed_t exposed = {0u};
    struct tm decodedDateTime = {0u};

    BOOST_CHECK(input.is_open());
    BOOST_CHECK(input.good());

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});

    BOOST_TEST_MESSAGE("read: " + std::to_string(buffer.size()));

    std::vector<uint8_t>::const_iterator from = buffer.begin();
    std::vector<uint8_t>::const_iterator to = buffer.begin() + sizeof(event_log_t);

    const std::vector<uint8_t>::const_iterator end = buffer.end();

    for (size_t i = 0; i < buffer.size(); i += sizeof(event_log_t)) {
    	std::copy(from, to, entry);

    	from += sizeof(event_log_t);
    	to += sizeof(event_log_t);

    	if (to == end) {
    		break;
    	}

    	const bool crcOk = _TestDumper::_convertEventToExposedEvent(event, exposed, decodedDateTime);

    	if (!crcOk) {
    		crcErrors++;
    	}
    }

    BOOST_CHECK_EQUAL(crcErrors, 40);
}

BOOST_AUTO_TEST_CASE(basic) {
	LogDumperTextFile textFile;

	event_log_exposed_t exposed = {0u};
	// 	else if (src == EVENT_SRC_MAIN && svrty == EVENT_INFO_CYCLIC && id == EVENTS_MAIN_CYCLIC) {
	exposed.severity = EVENT_INFO_CYCLIC;
	exposed.source = EVENT_SRC_MAIN;
	exposed.event_id = 0xAAu;

	exposed.event_master_time = 123456u;

	exposed.param = 1;
	exposed.param2 = 2;
	exposed.wparam = 3;
	exposed.wparam2 = 1288;
	exposed.wparam3 = 5;
	exposed.lparam = 4888;
	exposed.lparam2 = 4999;

	  time_t rawtime;
	  time (&rawtime);
	struct tm * timestamp = localtime(&rawtime);

	textFile.startTextExport("test");
	textFile.storeEntryInExport(&exposed, timestamp);
	textFile.closeAndSaveTextExport();
}
