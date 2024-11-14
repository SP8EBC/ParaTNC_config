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

	textFile.startTextExport("test");
}
