/*
 * LogDumper_test.cpp
 *
 *  Created on: Nov 8, 2024
 *      Author: mateusz
 */

#include <LogDumperTextFile.h>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LOGDUMPER
#include <boost/test/unit_test.hpp>


#include <vector>
#include <fstream>
#include <iostream>

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

BOOST_GLOBAL_FIXTURE (MyConfig);

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
