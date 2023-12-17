/*
 * ConfigEncodeDecode_test.cpp
 *
 *  Created on: Dec 16, 2023
 *      Author: mateusz
 */

#include "ProgramConfig.h"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DECODEENCODE
#include <boost/test/unit_test.hpp>
#include <boost/test/detail/log_level.hpp>

#include <fstream>

#include <vector>
#include <string>
#include <utility>

#include "../shared/config/encode/EncodeVer0.h"
#include "../shared/config/decode/DecodeVer0.h"

#include "../shared/config/DefaultConfig.h"

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/configencodedecode_test.log" )
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

BOOST_AUTO_TEST_CASE(beacon_description) {

	std::string test("test123");
	std::string test_output;

	EncodeVer0 encode;
	encode.setDescritpion(test);

	DecodeVer0 decode(encode.getData());
	decode.getDescritpion(test_output);

	BOOST_CHECK_EQUAL(test_output, test);
}

BOOST_AUTO_TEST_CASE(symbols) {
	EncodeVer0 encode;

	encode.setSymbol(SYMBOL_HOUSE);
	DecodeVer0 decode(encode.getData());
	BOOST_CHECK_EQUAL(decode.getSymbol(), SYMBOL_HOUSE);

	encode.setSymbol(SYMBOL_IGATE);
	DecodeVer0 decode2(encode.getData());
	BOOST_CHECK_EQUAL(decode2.getSymbol(), SYMBOL_IGATE);

	encode.setSymbol(SYMBOL_RXIGATE);
	DecodeVer0 decode3(encode.getData());
	BOOST_CHECK_EQUAL(decode3.getSymbol(), SYMBOL_RXIGATE);

	encode.setSymbol(SYMBOL_SAILBOAT);
	DecodeVer0 decode4(encode.getData());
	BOOST_CHECK_EQUAL(decode4.getSymbol(), SYMBOL_SAILBOAT);
}

BOOST_AUTO_TEST_CASE(defaults) {
	EncodeVer0 encode;
	encode.loadDefaults();

	DecodeVer0 decode(encode.getData());

	std::string description;
	decode.getDescritpion(description);
	BOOST_CHECK_EQUAL(description, DEFAULT_CONFIG_DESCRIPTION);

	std::string call;
	decode.getCallsign(call);
	BOOST_CHECK_EQUAL(call, DEFAULT_CONFIG_CALLSIGN);

	BOOST_CHECK_EQUAL(decode.getLatitude(), DEFAULT_CONFIG_LATITUDE);
	BOOST_CHECK_EQUAL(decode.getLongitude(), DEFAULT_CONFIG_LONGITUDE);
	BOOST_CHECK_EQUAL(decode.getSsid(), DEFAULT_CONFIG_SSID);
	BOOST_CHECK_EQUAL(decode.getNorS(), DEFAULT_CONFIG_N_OR_S);
	BOOST_CHECK_EQUAL(decode.getEorW(), DEFAULT_CONFIG_E_OR_W);
	BOOST_CHECK_EQUAL(decode.getSymbol(), DEFAULT_CONFIG_SYMBOL);
	BOOST_CHECK_EQUAL(decode.getPath(), DEFAULT_CONFIG_PATH);
	BOOST_CHECK_EQUAL(decode.getBeaconAtStartup(), DEFAULT_CONFIG_BEACON_START);
	BOOST_CHECK_EQUAL(decode.getWxTransmitPeriod(), DEFAULT_CONFIG_WX_PERIOD);
	BOOST_CHECK_EQUAL(decode.getBeaconTransmitPeriod(), DEFAULT_CONFIG_BEACON_PERIO);
	BOOST_CHECK_EQUAL(decode.getWxDoubleTransmit(), DEFAULT_CONFIG_WX_DOUBLE_TX);


}


