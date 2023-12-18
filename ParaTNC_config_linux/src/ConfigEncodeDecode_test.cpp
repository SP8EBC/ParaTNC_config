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
#include "../shared/config/decode/ValidateVer0.h"

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

	BOOST_CHECK_EQUAL(decode.getTemperatureSrc(), DEFAULT_CONFIG_SOURCE_TEMPERATURE);
	BOOST_CHECK_EQUAL(decode.getPressureSrc(), DEFAULT_CONFIG_SOURCE_PRESSURE);
	BOOST_CHECK_EQUAL(decode.getWindSrc(), DEFAULT_CONFIG_SOURCE_WIND);
	BOOST_CHECK_EQUAL(decode.getHumiditySrc(), DEFAULT_CONFIG_SOURCE_HUMIDITY);

	BOOST_CHECK_EQUAL(decode.getUmbSlaveClass(), DEFAULT_CONFIG_UMB_SLAVE_CLASS);
	BOOST_CHECK_EQUAL(decode.getUmbSlaveId(), DEFAULT_CONFIG_UMB_SLAVE_ID);
	BOOST_CHECK_EQUAL(decode.getUmbChannelWindspeed(), DEFAULT_CONFIG_UMB_CHANNEL_WINDSPEED);
	BOOST_CHECK_EQUAL(decode.getUmbChannelWinggusts(), DEFAULT_CONFIG_UMB_CHANNEL_WINDGUSTS);
	BOOST_CHECK_EQUAL(decode.getUmbChannelWinddirection(), DEFAULT_CONFIG_UMB_CHANNEL_WINDDIR);
	BOOST_CHECK_EQUAL(decode.getUmbChannelTemperature(), DEFAULT_CONDIG_UMB_CHANNEL_TEMPERATURE);
	BOOST_CHECK_EQUAL(decode.getUmbChannelQnh(), DEFAULT_CONFIG_UMB_CHANNEL_QNH);

	BOOST_CHECK_EQUAL(decode.getRtuSlaveSpeed(), DEFAULT_CONFIG_RTU_SLAVE_SPEED);
	BOOST_CHECK_EQUAL(decode.getRtuSlaveParity(), DEFAULT_CONFIG_RTU_SLAVE_PARITY);
	BOOST_CHECK_EQUAL(decode.getRtuSlaveStopBits(), DEFAULT_CONFIG_RTU_STOP_BITS);
	BOOST_CHECK_EQUAL(decode.getRtuFullWindData(), DEFAULT_CONFIG_RTU_FULL_WIND);
	BOOST_CHECK_EQUAL(decode.getRtuConfiguredSourceWindspeed(), DEFAULT_CONFIG_RTU_SOURCE_WINDSPEED);
	BOOST_CHECK_EQUAL(decode.getRtuConfiguredSourceWindgusts(), DEFAULT_CONFIG_RTU_SOURCE_WINDGUST);
	BOOST_CHECK_EQUAL(decode.getRtuConfiguredSourceWinddirection(), DEFAULT_CONFIG_RTU_SOURCE_WINDDIR);
	BOOST_CHECK_EQUAL(decode.getRtuConfiguredSourceTemperature(), DEFAULT_CONFIG_RTU_SOURCE_TEMP);
	BOOST_CHECK_EQUAL(decode.getRtuConfiguredSourceQnh(), DEFAULT_CONFIG_RTU_SOURCE_PRESSURE);

	BOOST_CHECK_EQUAL(decode.getGsmPin(), DEFAULT_CONFIG_GSM_PIN);
	BOOST_CHECK_EQUAL(decode.getGsmApiEnable(), DEFAULT_CONFIG_GSM_API_ENABLE);
	BOOST_CHECK_EQUAL(decode.getGsmAprsisEnable(), DEFAULT_CONFIG_APRSIS_ENABLE);
	BOOST_CHECK_EQUAL(decode.getGsmAprsisServerPort(), DEFAULT_CONFIG_APRSIS_PORT);
	BOOST_CHECK_EQUAL(decode.getGsmAprsisPasscode(), DEFAULT_CONFIG_APRSIS_PASSCODE);

	std::string apn_name;
	decode.getGsmApnName(apn_name);
	BOOST_CHECK_EQUAL(apn_name, DEFAULT_CONFIG_GSM_APNNAME);

	std::string apn_username;
	decode.getGsmApnUsername(apn_username);
	BOOST_CHECK_EQUAL(apn_username, DEFAULT_CONFIG_GSM_USERNAME);

	std::string apn_password;
	decode.getGsmApnPassword(apn_password);
	BOOST_CHECK_EQUAL(apn_password, DEFAULT_CONFIG_GSM_PASSWORD);

	std::string api_base_url;
	decode.getGsmApiBaseUrl(api_base_url);
	BOOST_CHECK_EQUAL(api_base_url, DEFAULT_CONFIG_GSM_API_BASEURL);

	std::string aprsis_server;
	decode.getGsmAprsisServer(aprsis_server);
	BOOST_CHECK_EQUAL(aprsis_server, DEFAULT_CONFIG_APRSIS_SERVER);

}

BOOST_AUTO_TEST_CASE(validate_by_calculating_crc) {
	EncodeVer0 encode;
	encode.loadDefaults();
	BOOST_CHECK_EQUAL(encode.incrementProgrammingCounter(), 2);

	ValidateVer0 validate;
	validate.calculateValidate(encode.getData());

	BOOST_CHECK(validate.checkValidate(encode.getData()));
}
