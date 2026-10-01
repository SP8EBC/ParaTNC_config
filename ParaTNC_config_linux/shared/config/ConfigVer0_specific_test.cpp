/*
 * ConfigVer0_specific_test.cpp
 *
 * Unit tests for user-friendly encoders/decoders defined in ConfigVer0_specific.h.
 * Build and run using Makefile located in the same directory: 'make test'
 */

// ConfigVer0_specific.h relies on these being included earlier
#include <algorithm>
#include <cctype>
#include <stdexcept>

#include "ConfigVer0_specific.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CONFIGVER0_SPECIFIC
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

// allows BOOST_CHECK_EQUAL to print enum class values
std::ostream &operator<< (std::ostream &os, ModeConfigWxValues v)
{
	return os << static_cast<int> (v);
}

std::ostream &operator<< (std::ostream &os, BasicConfigPtResistance v)
{
	return os << static_cast<int> (v);
}

std::ostream &operator<< (std::ostream &os, BasicConfigReferenceRes v)
{
	return os << static_cast<int> (v);
}

namespace {

/**
 * @brief Every reference resistor index together with the text used in configuration file
 */
const std::vector<std::pair<BasicConfigReferenceRes, std::string>> REFERENCE_RESISTORS = {
	{BasicConfigReferenceRes::_430, "430"},	  {BasicConfigReferenceRes::_432, "432"},
	{BasicConfigReferenceRes::_442, "442"},	  {BasicConfigReferenceRes::_470, "470"},
	{BasicConfigReferenceRes::_499, "499"},	  {BasicConfigReferenceRes::_510, "510"},
	{BasicConfigReferenceRes::_560, "560"},	  {BasicConfigReferenceRes::_620, "620"},
	{BasicConfigReferenceRes::_680, "680"},	  {BasicConfigReferenceRes::_768, "768"},
	{BasicConfigReferenceRes::_1000, "1000"}, {BasicConfigReferenceRes::_1100, "1100"},
	{BasicConfigReferenceRes::_1200, "1200"}, {BasicConfigReferenceRes::_1300, "1300"},
	{BasicConfigReferenceRes::_1400, "1400"}, {BasicConfigReferenceRes::_1500, "1500"},
	{BasicConfigReferenceRes::_1600, "1600"}, {BasicConfigReferenceRes::_1740, "1740"},
	{BasicConfigReferenceRes::_1800, "1800"}, {BasicConfigReferenceRes::_1910, "1910"},
	{BasicConfigReferenceRes::_2000, "2000"}, {BasicConfigReferenceRes::_2100, "2100"},
	{BasicConfigReferenceRes::_2400, "2400"}, {BasicConfigReferenceRes::_2700, "2700"},
	{BasicConfigReferenceRes::_3000, "3000"}, {BasicConfigReferenceRes::_3090, "3090"},
	{BasicConfigReferenceRes::_3400, "3400"}, {BasicConfigReferenceRes::_3900, "3900"},
	{BasicConfigReferenceRes::_4300, "4300"}, {BasicConfigReferenceRes::_4700, "4700"},
	{BasicConfigReferenceRes::_4990, "4990"}, {BasicConfigReferenceRes::_5600, "5600"},
};

/**
 * @brief Builds raw 'wx_pt_sensor' byte in the same layout as the controller uses
 */
uint8_t ptRaw (uint8_t type, uint8_t resistor)
{
	return static_cast<uint8_t> ((resistor << 2) | (type & 0x3U));
}

const std::vector<SourceConfigForWhat> ALL_FOR_WHAT = {
	SourceConfigForWhat::Wind, SourceConfigForWhat::Temperature, SourceConfigForWhat::Pressure,
	SourceConfigForWhat::Humidity};

} // namespace

// ============================================================================
// BasicConfigPtSensor
// ============================================================================

BOOST_AUTO_TEST_SUITE (basic_config_pt_sensor)

// ----------------------------------------------------------------------------
// BasicConfigPtSensor (uint8_t from)
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (from_uint8_zero_is_off)
{
	BasicConfigPtSensor pt (static_cast<uint8_t> (0x00U));
	BOOST_CHECK_EQUAL (pt.type, BasicConfigPtResistance::OFF);
}

BOOST_AUTO_TEST_CASE (from_uint8_ff_is_off)
{
	BasicConfigPtSensor pt (static_cast<uint8_t> (0xFFU));
	BOOST_CHECK_EQUAL (pt.type, BasicConfigPtResistance::OFF);
}

BOOST_AUTO_TEST_CASE (from_uint8_pt1000_all_resistors)
{
	for (const auto &r : REFERENCE_RESISTORS) {
		BasicConfigPtSensor pt (ptRaw (1U, static_cast<uint8_t> (r.first)));
		BOOST_CHECK_EQUAL (pt.type, BasicConfigPtResistance::PT1000);
		BOOST_CHECK_EQUAL (pt.referenceResistor, r.first);
	}
}

BOOST_AUTO_TEST_CASE (from_uint8_pt100_all_resistors)
{
	for (const auto &r : REFERENCE_RESISTORS) {
		BasicConfigPtSensor pt (ptRaw (3U, static_cast<uint8_t> (r.first)));
		BOOST_CHECK_EQUAL (pt.type, BasicConfigPtResistance::PT100);
		BOOST_CHECK_EQUAL (pt.referenceResistor, r.first);
	}
}

BOOST_AUTO_TEST_CASE (from_uint8_type_bits_zero_is_off)
{
	BasicConfigPtSensor pt (ptRaw (0U, static_cast<uint8_t> (BasicConfigReferenceRes::_1000)));
	BOOST_CHECK_EQUAL (pt.type, BasicConfigPtResistance::OFF);
}

// type bits == 2 are reserved
BOOST_AUTO_TEST_CASE (from_uint8_reserved_type_is_off)
{
	BasicConfigPtSensor pt (ptRaw (2U, static_cast<uint8_t> (BasicConfigReferenceRes::_1000)));
	BOOST_CHECK_EQUAL (pt.type, BasicConfigPtResistance::OFF);
	BOOST_CHECK_EQUAL (pt.referenceResistor, BasicConfigReferenceRes::_4300);
}

// resistor index 32..63 is outside of lookup table
BOOST_AUTO_TEST_CASE (from_uint8_resistor_out_of_table_is_off)
{
	for (uint8_t resistor = 32U; resistor < 64U; resistor++) {
		BasicConfigPtSensor pt100 (ptRaw (3U, resistor));
		BOOST_CHECK_EQUAL (pt100.type, BasicConfigPtResistance::OFF);
		BOOST_CHECK_EQUAL (pt100.referenceResistor, BasicConfigReferenceRes::_4300);

		BasicConfigPtSensor pt1000 (ptRaw (1U, resistor));
		BOOST_CHECK_EQUAL (pt1000.type, BasicConfigPtResistance::OFF);
		BOOST_CHECK_EQUAL (pt1000.referenceResistor, BasicConfigReferenceRes::_4300);
	}
}

// ----------------------------------------------------------------------------
// toString
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (to_string_off)
{
	BasicConfigPtSensor pt (static_cast<uint8_t> (0x00U));
	BOOST_CHECK_EQUAL (pt.toString (), "false");
}

BOOST_AUTO_TEST_CASE (to_string_all_resistors)
{
	for (const auto &r : REFERENCE_RESISTORS) {
		BasicConfigPtSensor pt100 (ptRaw (3U, static_cast<uint8_t> (r.first)));
		BOOST_CHECK_EQUAL (pt100.toString (), "PT100_" + r.second);

		BasicConfigPtSensor pt1000 (ptRaw (1U, static_cast<uint8_t> (r.first)));
		BOOST_CHECK_EQUAL (pt1000.toString (), "PT1000_" + r.second);
	}
}

BOOST_AUTO_TEST_CASE (to_string_invalid_type_is_false)
{
	BasicConfigPtSensor pt (static_cast<uint8_t> (0x00U));
	pt.type = static_cast<BasicConfigPtResistance> (2);
	BOOST_CHECK_EQUAL (pt.toString (), "false");
}

BOOST_AUTO_TEST_CASE (to_string_invalid_resistor_is_zero)
{
	BasicConfigPtSensor pt (static_cast<uint8_t> (0x00U));
	pt.type = BasicConfigPtResistance::PT100;
	pt.referenceResistor = static_cast<BasicConfigReferenceRes> (40);
	BOOST_CHECK_EQUAL (pt.toString (), "PT100_0");
}

// ----------------------------------------------------------------------------
// BasicConfigPtSensor (std::string from)
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (from_string_false_is_off)
{
	BOOST_CHECK_EQUAL (BasicConfigPtSensor (std::string ("false")).type,
					   BasicConfigPtResistance::OFF);
	BOOST_CHECK_EQUAL (BasicConfigPtSensor (std::string ("FALSE")).type,
					   BasicConfigPtResistance::OFF);
}

BOOST_AUTO_TEST_CASE (from_string_all_resistors)
{
	for (const auto &r : REFERENCE_RESISTORS) {
		BasicConfigPtSensor pt100 (std::string ("PT100_") + r.second);
		BOOST_CHECK_EQUAL (pt100.type, BasicConfigPtResistance::PT100);
		BOOST_CHECK_EQUAL (pt100.referenceResistor, r.first);

		BasicConfigPtSensor pt1000 (std::string ("PT1000_") + r.second);
		BOOST_CHECK_EQUAL (pt1000.type, BasicConfigPtResistance::PT1000);
		BOOST_CHECK_EQUAL (pt1000.referenceResistor, r.first);
	}
}

BOOST_AUTO_TEST_CASE (from_string_is_case_insensitive)
{
	BasicConfigPtSensor pt (std::string ("pt1000_1910"));
	BOOST_CHECK_EQUAL (pt.type, BasicConfigPtResistance::PT1000);
	BOOST_CHECK_EQUAL (pt.referenceResistor, BasicConfigReferenceRes::_1910);
}

BOOST_AUTO_TEST_CASE (from_string_type_only_uses_default_resistor)
{
	BasicConfigPtSensor pt100 (std::string ("PT100"));
	BOOST_CHECK_EQUAL (pt100.type, BasicConfigPtResistance::PT100);
	BOOST_CHECK_EQUAL (pt100.referenceResistor, BasicConfigReferenceRes::_4300);

	BasicConfigPtSensor pt1000 (std::string ("pt1000"));
	BOOST_CHECK_EQUAL (pt1000.type, BasicConfigPtResistance::PT1000);
	BOOST_CHECK_EQUAL (pt1000.referenceResistor, BasicConfigReferenceRes::_4300);
}

BOOST_AUTO_TEST_CASE (from_string_unknown_resistor_uses_default)
{
	BasicConfigPtSensor pt (std::string ("PT100_1234"));
	BOOST_CHECK_EQUAL (pt.type, BasicConfigPtResistance::PT100);
	BOOST_CHECK_EQUAL (pt.referenceResistor, BasicConfigReferenceRes::_4300);

	BasicConfigPtSensor empty (std::string ("PT1000_"));
	BOOST_CHECK_EQUAL (empty.type, BasicConfigPtResistance::PT1000);
	BOOST_CHECK_EQUAL (empty.referenceResistor, BasicConfigReferenceRes::_4300);
}

BOOST_AUTO_TEST_CASE (from_string_malformed_is_off)
{
	BOOST_CHECK_EQUAL (BasicConfigPtSensor (std::string ("")).type, BasicConfigPtResistance::OFF);
	BOOST_CHECK_EQUAL (BasicConfigPtSensor (std::string ("garbage")).type,
					   BasicConfigPtResistance::OFF);
	BOOST_CHECK_EQUAL (BasicConfigPtSensor (std::string ("PT500_1000")).type,
					   BasicConfigPtResistance::OFF);
	BOOST_CHECK_EQUAL (BasicConfigPtSensor (std::string ("_1000")).type,
					   BasicConfigPtResistance::OFF);
	BOOST_CHECK_EQUAL (BasicConfigPtSensor (std::string ("true")).type,
					   BasicConfigPtResistance::OFF);
}

// ----------------------------------------------------------------------------
// toUint8
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (to_uint8_off_is_zero)
{
	BOOST_CHECK_EQUAL (BasicConfigPtSensor (std::string ("false")).toUint8 (), 0x00U);

	// OFF with non default reference resistor must still give zero
	BasicConfigPtSensor pt (ptRaw (0U, static_cast<uint8_t> (BasicConfigReferenceRes::_1000)));
	BOOST_CHECK_EQUAL (pt.toUint8 (), 0x00U);
}

BOOST_AUTO_TEST_CASE (to_uint8_layout)
{
	BasicConfigPtSensor pt (std::string ("PT1000_1000"));
	BOOST_CHECK_EQUAL (pt.toUint8 (), (10U << 2) | 1U);

	BasicConfigPtSensor pt100 (std::string ("PT100_5600"));
	BOOST_CHECK_EQUAL (pt100.toUint8 (), (31U << 2) | 3U);
}

// ----------------------------------------------------------------------------
// round trips
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (round_trip_uint8_string_uint8)
{
	for (const auto &r : REFERENCE_RESISTORS) {
		for (uint8_t type : {1U, 3U}) {
			const uint8_t raw = ptRaw (type, static_cast<uint8_t> (r.first));

			BasicConfigPtSensor fromRaw (raw);
			BasicConfigPtSensor fromText (fromRaw.toString ());

			BOOST_CHECK_EQUAL (fromText.toUint8 (), raw);
		}
	}
}

BOOST_AUTO_TEST_CASE (round_trip_off)
{
	BasicConfigPtSensor fromRaw (static_cast<uint8_t> (0xFFU));
	BasicConfigPtSensor fromText (fromRaw.toString ());
	BOOST_CHECK_EQUAL (fromText.toUint8 (), 0x00U);
}

BOOST_AUTO_TEST_SUITE_END ()

// ============================================================================
// SourceConfigSourceConfig
// ============================================================================

BOOST_AUTO_TEST_SUITE (source_config_source_config)

// ----------------------------------------------------------------------------
// SourceConfigSourceConfig (uint8_t from, SourceConfigForWhat what)
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (from_uint8_wind_valid)
{
	for (auto src : {WX_SOURCE_INTERNAL, WX_SOURCE_UMB, WX_SOURCE_RTU, WX_SOURCE_FULL_RTU,
					 WX_SOURCE_DAVIS_SERIAL}) {
		SourceConfigSourceConfig s (static_cast<uint8_t> (src), SourceConfigForWhat::Wind);
		BOOST_CHECK_EQUAL (s.source, src);
		BOOST_CHECK (s.forWhat == SourceConfigForWhat::Wind);
	}
}

BOOST_AUTO_TEST_CASE (from_uint8_wind_pt100_throws)
{
	BOOST_CHECK_THROW (SourceConfigSourceConfig (static_cast<uint8_t> (WX_SOURCE_INTERNAL_PT100),
												 SourceConfigForWhat::Wind),
					   std::out_of_range);
}

BOOST_AUTO_TEST_CASE (from_uint8_temperature_valid)
{
	for (auto src : {WX_SOURCE_INTERNAL, WX_SOURCE_INTERNAL_PT100, WX_SOURCE_UMB, WX_SOURCE_RTU,
					 WX_SOURCE_DAVIS_SERIAL}) {
		SourceConfigSourceConfig s (static_cast<uint8_t> (src), SourceConfigForWhat::Temperature);
		BOOST_CHECK_EQUAL (s.source, src);
	}
}

BOOST_AUTO_TEST_CASE (from_uint8_pressure_humidity_valid)
{
	for (auto what : {SourceConfigForWhat::Pressure, SourceConfigForWhat::Humidity}) {
		for (auto src : {WX_SOURCE_INTERNAL, WX_SOURCE_UMB, WX_SOURCE_RTU, WX_SOURCE_DAVIS_SERIAL}) {
			SourceConfigSourceConfig s (static_cast<uint8_t> (src), what);
			BOOST_CHECK_EQUAL (s.source, src);
		}
	}
}

BOOST_AUTO_TEST_CASE (from_uint8_pressure_humidity_pt100_throws)
{
	BOOST_CHECK_THROW (SourceConfigSourceConfig (static_cast<uint8_t> (WX_SOURCE_INTERNAL_PT100),
												 SourceConfigForWhat::Pressure),
					   std::out_of_range);
	BOOST_CHECK_THROW (SourceConfigSourceConfig (static_cast<uint8_t> (WX_SOURCE_INTERNAL_PT100),
												 SourceConfigForWhat::Humidity),
					   std::out_of_range);
}

// values which are not a member of config_data_wx_sources_enum_t at all
BOOST_AUTO_TEST_CASE (from_uint8_undefined_value_throws)
{
	for (auto what : ALL_FOR_WHAT) {
		for (uint8_t raw : {0x00U, 0x07U, 0x80U, 0xFFU}) {
			BOOST_TEST_CONTEXT ("what: " << static_cast<int> (what) << ", raw: " << (int)raw)
			{
				BOOST_CHECK_THROW (SourceConfigSourceConfig (raw, what), std::out_of_range);
			}
		}
	}
}

// whatever is accepted from NvMem must be possible to store in text configuration
BOOST_AUTO_TEST_CASE (from_uint8_accepted_value_can_be_converted_to_string)
{
	for (auto what : ALL_FOR_WHAT) {
		for (int raw = 0; raw <= 0xFF; raw++) {
			try {
				SourceConfigSourceConfig s (static_cast<uint8_t> (raw), what);
				BOOST_TEST_CONTEXT ("what: " << static_cast<int> (what) << ", raw: " << raw)
				{
					BOOST_CHECK_NO_THROW (s.toString ());
				}
			}
			catch (const std::out_of_range &) {
				; // rejected by constructor, this is fine
			}
		}
	}
}

// ----------------------------------------------------------------------------
// toString
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (to_string_internal)
{
	BOOST_CHECK_EQUAL (SourceConfigSourceConfig (static_cast<uint8_t> (WX_SOURCE_INTERNAL),
												 SourceConfigForWhat::Wind)
						   .toString (),
					   "DAVIS");
	BOOST_CHECK_EQUAL (SourceConfigSourceConfig (static_cast<uint8_t> (WX_SOURCE_INTERNAL),
												 SourceConfigForWhat::Temperature)
						   .toString (),
					   "1WIRE");
	BOOST_CHECK_EQUAL (SourceConfigSourceConfig (static_cast<uint8_t> (WX_SOURCE_INTERNAL),
												 SourceConfigForWhat::Pressure)
						   .toString (),
					   "INTERNAL");
	BOOST_CHECK_EQUAL (SourceConfigSourceConfig (static_cast<uint8_t> (WX_SOURCE_INTERNAL),
												 SourceConfigForWhat::Humidity)
						   .toString (),
					   "INTERNAL");
}

BOOST_AUTO_TEST_CASE (to_string_pt100)
{
	BOOST_CHECK_EQUAL (SourceConfigSourceConfig (static_cast<uint8_t> (WX_SOURCE_INTERNAL_PT100),
												 SourceConfigForWhat::Temperature)
						   .toString (),
					   "PT1000");
}

BOOST_AUTO_TEST_CASE (to_string_pt100_not_for_temperature_throws)
{
	SourceConfigSourceConfig s (static_cast<uint8_t> (WX_SOURCE_INTERNAL_PT100),
								SourceConfigForWhat::Temperature);
	for (auto what :
		 {SourceConfigForWhat::Wind, SourceConfigForWhat::Pressure, SourceConfigForWhat::Humidity}) {
		s.forWhat = what;
		BOOST_CHECK_THROW (s.toString (), std::out_of_range);
	}
}

BOOST_AUTO_TEST_CASE (to_string_same_for_every_parameter)
{
	for (auto what : ALL_FOR_WHAT) {
		BOOST_CHECK_EQUAL (SourceConfigSourceConfig (static_cast<uint8_t> (WX_SOURCE_UMB), what)
							   .toString (),
						   "UMB");
		BOOST_CHECK_EQUAL (SourceConfigSourceConfig (static_cast<uint8_t> (WX_SOURCE_RTU), what)
							   .toString (),
						   "MODBUS");
		BOOST_CHECK_EQUAL (
			SourceConfigSourceConfig (static_cast<uint8_t> (WX_SOURCE_DAVIS_SERIAL), what)
				.toString (),
			"DAVIS_SERIAL_LOGGER");
	}
}

BOOST_AUTO_TEST_CASE (to_string_full_rtu_wind)
{
	BOOST_CHECK_EQUAL (SourceConfigSourceConfig (static_cast<uint8_t> (WX_SOURCE_FULL_RTU),
												 SourceConfigForWhat::Wind)
						   .toString (),
					   "MODBUS_FULL");
}

BOOST_AUTO_TEST_CASE (to_string_full_rtu_not_for_wind_throws)
{
	SourceConfigSourceConfig s (static_cast<uint8_t> (WX_SOURCE_FULL_RTU),
								SourceConfigForWhat::Wind);
	for (auto what : {SourceConfigForWhat::Temperature, SourceConfigForWhat::Pressure,
					  SourceConfigForWhat::Humidity}) {
		s.forWhat = what;
		BOOST_CHECK_THROW (s.toString (), std::out_of_range);
	}
}

BOOST_AUTO_TEST_CASE (to_string_invalid_source_throws)
{
	SourceConfigSourceConfig s (static_cast<uint8_t> (WX_SOURCE_UMB), SourceConfigForWhat::Wind);
	s.source = static_cast<config_data_wx_sources_enum_t> (0x55);
	BOOST_CHECK_THROW (s.toString (), std::out_of_range);
}

BOOST_AUTO_TEST_CASE (to_string_invalid_for_what_throws)
{
	SourceConfigSourceConfig s (static_cast<uint8_t> (WX_SOURCE_INTERNAL),
								SourceConfigForWhat::Wind);
	s.forWhat = static_cast<SourceConfigForWhat> (0x55);
	BOOST_CHECK_THROW (s.toString (), std::out_of_range);
}

// ----------------------------------------------------------------------------
// SourceConfigSourceConfig (std::string from, SourceConfigForWhat what)
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (from_string_valid)
{
	const std::vector<std::pair<std::string, config_data_wx_sources_enum_t>> cases = {
		{"DAVIS", WX_SOURCE_INTERNAL},
		{"1WIRE", WX_SOURCE_INTERNAL},
		{"INTERNAL", WX_SOURCE_INTERNAL},
		{"PT1000", WX_SOURCE_INTERNAL_PT100},
		{"UMB", WX_SOURCE_UMB},
		{"MODBUS", WX_SOURCE_RTU},
		{"MODBUS_FULL", WX_SOURCE_FULL_RTU},
		{"DAVIS_SERIAL_LOGGER", WX_SOURCE_DAVIS_SERIAL},
	};

	for (const auto &c : cases) {
		SourceConfigSourceConfig s (c.first, SourceConfigForWhat::Temperature);
		BOOST_CHECK_EQUAL (s.source, c.second);
		BOOST_CHECK (s.forWhat == SourceConfigForWhat::Temperature);
	}
}

BOOST_AUTO_TEST_CASE (from_string_is_case_insensitive)
{
	BOOST_CHECK_EQUAL (SourceConfigSourceConfig (std::string ("modbus_full"),
												 SourceConfigForWhat::Wind)
						   .source,
					   WX_SOURCE_FULL_RTU);
	BOOST_CHECK_EQUAL (
		SourceConfigSourceConfig (std::string ("Davis_Serial_Logger"), SourceConfigForWhat::Wind)
			.source,
		WX_SOURCE_DAVIS_SERIAL);
	BOOST_CHECK_EQUAL (
		SourceConfigSourceConfig (std::string ("1wire"), SourceConfigForWhat::Temperature).source,
		WX_SOURCE_INTERNAL);
}

BOOST_AUTO_TEST_CASE (from_string_unknown_throws)
{
	for (const char *str : {"", "DAVIS ", "MODBUSFULL", "PT100", "garbage"}) {
		BOOST_CHECK_THROW (SourceConfigSourceConfig (str, SourceConfigForWhat::Wind),
						   std::out_of_range);
	}
}

// ----------------------------------------------------------------------------
// toUint8 / round trips
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (to_uint8)
{
	for (auto src : {WX_SOURCE_INTERNAL, WX_SOURCE_UMB, WX_SOURCE_RTU, WX_SOURCE_FULL_RTU,
					 WX_SOURCE_DAVIS_SERIAL}) {
		SourceConfigSourceConfig s (static_cast<uint8_t> (src), SourceConfigForWhat::Wind);
		BOOST_CHECK_EQUAL (s.toUint8 (), static_cast<uint8_t> (src));
	}
}

BOOST_AUTO_TEST_CASE (round_trip_uint8_string_uint8)
{
	const std::vector<std::pair<SourceConfigForWhat, std::vector<config_data_wx_sources_enum_t>>>
		cases = {
			{SourceConfigForWhat::Wind,
			 {WX_SOURCE_INTERNAL, WX_SOURCE_UMB, WX_SOURCE_RTU, WX_SOURCE_FULL_RTU,
			  WX_SOURCE_DAVIS_SERIAL}},
			{SourceConfigForWhat::Temperature,
			 {WX_SOURCE_INTERNAL, WX_SOURCE_INTERNAL_PT100, WX_SOURCE_UMB, WX_SOURCE_RTU,
			  WX_SOURCE_DAVIS_SERIAL}},
			{SourceConfigForWhat::Pressure,
			 {WX_SOURCE_INTERNAL, WX_SOURCE_UMB, WX_SOURCE_RTU, WX_SOURCE_DAVIS_SERIAL}},
			{SourceConfigForWhat::Humidity,
			 {WX_SOURCE_INTERNAL, WX_SOURCE_UMB, WX_SOURCE_RTU, WX_SOURCE_DAVIS_SERIAL}},
		};

	for (const auto &c : cases) {
		for (auto src : c.second) {
			SourceConfigSourceConfig fromRaw (static_cast<uint8_t> (src), c.first);
			SourceConfigSourceConfig fromText (fromRaw.toString (), c.first);
			BOOST_CHECK_EQUAL (fromText.toUint8 (), static_cast<uint8_t> (src));
		}
	}
}

BOOST_AUTO_TEST_SUITE_END ()

// ============================================================================
// ModeConfigPowersave
// ============================================================================

BOOST_AUTO_TEST_SUITE (mode_config_powersave)

BOOST_AUTO_TEST_CASE (from_uint8_valid)
{
	BOOST_CHECK_EQUAL (ModeConfigPowersave (static_cast<uint8_t> (0U)).powersave, PWSAVE_NONE);
	BOOST_CHECK_EQUAL (ModeConfigPowersave (static_cast<uint8_t> (1U)).powersave, PWSAVE_NORMAL);
	BOOST_CHECK_EQUAL (ModeConfigPowersave (static_cast<uint8_t> (3U)).powersave, PWSAVE_AGGRESV);
}

BOOST_AUTO_TEST_CASE (from_uint8_invalid_throws)
{
	for (uint8_t raw : {0x02U, 0x04U, 0x7FU, 0xFEU, 0xFFU}) {
		BOOST_CHECK_THROW (ModeConfigPowersave{raw}, std::out_of_range);
	}
}

BOOST_AUTO_TEST_CASE (to_string)
{
	BOOST_CHECK_EQUAL (ModeConfigPowersave (static_cast<uint8_t> (PWSAVE_NONE)).toString (),
					   "NONE");
	BOOST_CHECK_EQUAL (ModeConfigPowersave (static_cast<uint8_t> (PWSAVE_NORMAL)).toString (),
					   "NORMAL");
	BOOST_CHECK_EQUAL (ModeConfigPowersave (static_cast<uint8_t> (PWSAVE_AGGRESV)).toString (),
					   "AGGRESIVE");
}

BOOST_AUTO_TEST_CASE (to_string_invalid_throws)
{
	ModeConfigPowersave p (static_cast<uint8_t> (PWSAVE_NONE));
	p.powersave = PWSAVE_NULL;
	BOOST_CHECK_THROW (p.toString (), std::out_of_range);
}

BOOST_AUTO_TEST_CASE (from_string_valid)
{
	BOOST_CHECK_EQUAL (ModeConfigPowersave (std::string ("NONE")).powersave, PWSAVE_NONE);
	BOOST_CHECK_EQUAL (ModeConfigPowersave (std::string ("NORMAL")).powersave, PWSAVE_NORMAL);
	BOOST_CHECK_EQUAL (ModeConfigPowersave (std::string ("AGGRESIVE")).powersave, PWSAVE_AGGRESV);
}

BOOST_AUTO_TEST_CASE (from_string_is_case_insensitive)
{
	BOOST_CHECK_EQUAL (ModeConfigPowersave (std::string ("none")).powersave, PWSAVE_NONE);
	BOOST_CHECK_EQUAL (ModeConfigPowersave (std::string ("Normal")).powersave, PWSAVE_NORMAL);
	BOOST_CHECK_EQUAL (ModeConfigPowersave (std::string ("aggresive")).powersave, PWSAVE_AGGRESV);
}

BOOST_AUTO_TEST_CASE (from_string_invalid_throws)
{
	for (const char *str : {"", "AGGRESSIVE", "NULL", "OFF", " NONE", "1"}) {
		BOOST_CHECK_THROW (ModeConfigPowersave{str}, std::out_of_range);
	}
}

BOOST_AUTO_TEST_CASE (to_uint8)
{
	BOOST_CHECK_EQUAL (ModeConfigPowersave (std::string ("NONE")).toUint8 (), 0U);
	BOOST_CHECK_EQUAL (ModeConfigPowersave (std::string ("NORMAL")).toUint8 (), 1U);
	BOOST_CHECK_EQUAL (ModeConfigPowersave (std::string ("AGGRESIVE")).toUint8 (), 3U);
}

BOOST_AUTO_TEST_CASE (round_trip_uint8_string_uint8)
{
	for (uint8_t raw : {0U, 1U, 3U}) {
		ModeConfigPowersave fromRaw (raw);
		ModeConfigPowersave fromText (fromRaw.toString ());
		BOOST_CHECK_EQUAL (fromText.toUint8 (), raw);
	}
}

BOOST_AUTO_TEST_SUITE_END ()

// ============================================================================
// ModeConfigWx
// ============================================================================

BOOST_AUTO_TEST_SUITE (mode_config_wx)

// ----------------------------------------------------------------------------
// toString
// ----------------------------------------------------------------------------

namespace {
ModeConfigWx makeWx (ModeConfigWxValues v)
{
	ModeConfigWx wx (static_cast<uint8_t> (0U));
	wx.mode = v;
	return wx;
}
} // namespace

BOOST_AUTO_TEST_CASE (to_string_disabled)
{
	BOOST_CHECK_EQUAL (makeWx (ModeConfigWxValues::Disabled).toString (), "OFF");
}

BOOST_AUTO_TEST_CASE (to_string_enabled)
{
	BOOST_CHECK_EQUAL (makeWx (ModeConfigWxValues::Enabled).toString (), "ON");
}

BOOST_AUTO_TEST_CASE (to_string_enabled_with_onewire)
{
	BOOST_CHECK_EQUAL (makeWx (ModeConfigWxValues::EnabledWithOneWire).toString (), "ON_ONEWIRE");
}

BOOST_AUTO_TEST_CASE (to_string_enabled_with_validator)
{
	BOOST_CHECK_EQUAL (makeWx (ModeConfigWxValues::EnabledWithValidator).toString (),
					   "ON_VALIDATOR");
}

BOOST_AUTO_TEST_CASE (to_string_enabled_with_validator_and_onewire)
{
	BOOST_CHECK_EQUAL (makeWx (ModeConfigWxValues::EnabledWithValidatorAndOneWire).toString (),
					   "ON_VALIDATOR_ONEWIRE");
}

BOOST_AUTO_TEST_CASE (to_string_invalid_mode_throws)
{
	ModeConfigWx wx = makeWx (static_cast<ModeConfigWxValues> (0x55));
	BOOST_CHECK_THROW (wx.toString (), std::out_of_range);
}

// ----------------------------------------------------------------------------
// ModeConfigWx (uint8_t from) - conversion from NvMem bitmask
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (from_uint8_zero_is_disabled)
{
	ModeConfigWx wx (static_cast<uint8_t> (0U));
	BOOST_CHECK_EQUAL (wx.mode, ModeConfigWxValues::Disabled);
}

BOOST_AUTO_TEST_CASE (from_uint8_enabled_only_is_enabled_with_onewire)
{
	ModeConfigWx wx (static_cast<uint8_t> (WX_ENABLED));
	BOOST_CHECK_EQUAL (wx.mode, ModeConfigWxValues::EnabledWithOneWire);
}

BOOST_AUTO_TEST_CASE (from_uint8_enabled_dallas_disabled_is_enabled)
{
	ModeConfigWx wx (static_cast<uint8_t> (WX_ENABLED | WX_INTERNAL_DISABLE_DALLAS));
	BOOST_CHECK_EQUAL (wx.mode, ModeConfigWxValues::Enabled);
}

BOOST_AUTO_TEST_CASE (from_uint8_enabled_validator_is_enabled_with_validator_and_onewire)
{
	ModeConfigWx wx (static_cast<uint8_t> (WX_ENABLED | WX_CHECK_VALIDATE_PARAMS));
	BOOST_CHECK_EQUAL (wx.mode, ModeConfigWxValues::EnabledWithValidatorAndOneWire);
}

BOOST_AUTO_TEST_CASE (from_uint8_enabled_validator_dallas_disabled_is_enabled_with_validator)
{
	ModeConfigWx wx (static_cast<uint8_t> (WX_ENABLED | WX_INTERNAL_DISABLE_DALLAS |
										   WX_CHECK_VALIDATE_PARAMS));
	BOOST_CHECK_EQUAL (wx.mode, ModeConfigWxValues::EnabledWithValidator);
}

// bits not related to dallas or validator must not affect the result
BOOST_AUTO_TEST_CASE (from_uint8_unrelated_bits_are_ignored)
{
	const uint8_t unrelated = WX_INTERNAL_AS_BACKUP | WX_INTERNAL_SPARKFUN_WIND;

	BOOST_CHECK_EQUAL (ModeConfigWx (static_cast<uint8_t> (WX_ENABLED | unrelated)).mode,
					   ModeConfigWxValues::EnabledWithOneWire);
	BOOST_CHECK_EQUAL (
		ModeConfigWx (static_cast<uint8_t> (WX_ENABLED | WX_INTERNAL_DISABLE_DALLAS | unrelated))
			.mode,
		ModeConfigWxValues::Enabled);
	BOOST_CHECK_EQUAL (
		ModeConfigWx (static_cast<uint8_t> (WX_ENABLED | WX_CHECK_VALIDATE_PARAMS | unrelated))
			.mode,
		ModeConfigWxValues::EnabledWithValidatorAndOneWire);
	BOOST_CHECK_EQUAL (ModeConfigWx (static_cast<uint8_t> (WX_ENABLED | WX_INTERNAL_DISABLE_DALLAS |
														   WX_CHECK_VALIDATE_PARAMS | unrelated))
						   .mode,
					   ModeConfigWxValues::EnabledWithValidator);
}

// options set without WX_ENABLED bit are a broken configuration
BOOST_AUTO_TEST_CASE (from_uint8_options_without_enabled_bit_throws)
{
	BOOST_CHECK_THROW (ModeConfigWx (static_cast<uint8_t> (WX_INTERNAL_DISABLE_DALLAS)),
					   std::out_of_range);
	BOOST_CHECK_THROW (ModeConfigWx (static_cast<uint8_t> (WX_CHECK_VALIDATE_PARAMS)),
					   std::out_of_range);
	BOOST_CHECK_THROW (ModeConfigWx (static_cast<uint8_t> (WX_INTERNAL_DISABLE_DALLAS |
														   WX_CHECK_VALIDATE_PARAMS)),
					   std::out_of_range);
	BOOST_CHECK_THROW (ModeConfigWx (static_cast<uint8_t> (WX_INTERNAL_AS_BACKUP)),
					   std::out_of_range);
}

// bits above WX_CHECK_VALIDATE_PARAMS are not defined
BOOST_AUTO_TEST_CASE (from_uint8_undefined_bits_throws)
{
	BOOST_CHECK_THROW (ModeConfigWx (static_cast<uint8_t> (WX_ENABLED | (1 << 5))),
					   std::out_of_range);
	BOOST_CHECK_THROW (ModeConfigWx (static_cast<uint8_t> (WX_ENABLED | (1 << 6))),
					   std::out_of_range);
	BOOST_CHECK_THROW (ModeConfigWx (static_cast<uint8_t> (WX_ENABLED | (1 << 7))),
					   std::out_of_range);
	BOOST_CHECK_THROW (ModeConfigWx (static_cast<uint8_t> (0xFFU)), std::out_of_range);
}

// ----------------------------------------------------------------------------
// round trip: NvMem bitmask -> internal -> text
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (from_uint8_to_string)
{
	BOOST_CHECK_EQUAL (ModeConfigWx (static_cast<uint8_t> (0U)).toString (), "OFF");
	BOOST_CHECK_EQUAL (ModeConfigWx (static_cast<uint8_t> (WX_ENABLED)).toString (), "ON_ONEWIRE");
	BOOST_CHECK_EQUAL (
		ModeConfigWx (static_cast<uint8_t> (WX_ENABLED | WX_INTERNAL_DISABLE_DALLAS)).toString (),
		"ON");
	BOOST_CHECK_EQUAL (
		ModeConfigWx (static_cast<uint8_t> (WX_ENABLED | WX_CHECK_VALIDATE_PARAMS)).toString (),
		"ON_VALIDATOR_ONEWIRE");
	BOOST_CHECK_EQUAL (ModeConfigWx (static_cast<uint8_t> (WX_ENABLED | WX_INTERNAL_DISABLE_DALLAS |
														   WX_CHECK_VALIDATE_PARAMS))
						   .toString (),
					   "ON_VALIDATOR");
}

// ----------------------------------------------------------------------------
// ModeConfigWx (std::string from)
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (from_string_valid)
{
	BOOST_CHECK_EQUAL (ModeConfigWx (std::string ("OFF")).mode, ModeConfigWxValues::Disabled);
	BOOST_CHECK_EQUAL (ModeConfigWx (std::string ("ON")).mode, ModeConfigWxValues::Enabled);
	BOOST_CHECK_EQUAL (ModeConfigWx (std::string ("ON_ONEWIRE")).mode,
					   ModeConfigWxValues::EnabledWithOneWire);
	BOOST_CHECK_EQUAL (ModeConfigWx (std::string ("ON_VALIDATOR")).mode,
					   ModeConfigWxValues::EnabledWithValidator);
	BOOST_CHECK_EQUAL (ModeConfigWx (std::string ("ON_VALIDATOR_ONEWIRE")).mode,
					   ModeConfigWxValues::EnabledWithValidatorAndOneWire);
}

BOOST_AUTO_TEST_CASE (from_string_alias_onewire_validator)
{
	BOOST_CHECK_EQUAL (ModeConfigWx (std::string ("ON_ONEWIRE_VALIDATOR")).mode,
					   ModeConfigWxValues::EnabledWithValidatorAndOneWire);
}

BOOST_AUTO_TEST_CASE (from_string_is_case_insensitive)
{
	BOOST_CHECK_EQUAL (ModeConfigWx (std::string ("off")).mode, ModeConfigWxValues::Disabled);
	BOOST_CHECK_EQUAL (ModeConfigWx (std::string ("On_Validator")).mode,
					   ModeConfigWxValues::EnabledWithValidator);
}

BOOST_AUTO_TEST_CASE (from_string_invalid_throws)
{
	for (const char *str : {"", "TRUE", "1", "ON_", "ON_VALIDATOR_", " ON", "ENABLED"}) {
		BOOST_CHECK_THROW (ModeConfigWx{str}, std::out_of_range);
	}
}

// ----------------------------------------------------------------------------
// toUint8
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (to_uint8_disabled_is_zero)
{
	BOOST_CHECK_EQUAL (makeWx (ModeConfigWxValues::Disabled).toUint8 (), 0U);
}

BOOST_AUTO_TEST_CASE (to_uint8_enabled_modes)
{
	BOOST_CHECK_EQUAL (makeWx (ModeConfigWxValues::Enabled).toUint8 (),
					   WX_ENABLED | WX_INTERNAL_DISABLE_DALLAS);
	BOOST_CHECK_EQUAL (makeWx (ModeConfigWxValues::EnabledWithOneWire).toUint8 (), WX_ENABLED);
	BOOST_CHECK_EQUAL (makeWx (ModeConfigWxValues::EnabledWithValidator).toUint8 (),
					   WX_ENABLED | WX_INTERNAL_DISABLE_DALLAS | WX_CHECK_VALIDATE_PARAMS);
	BOOST_CHECK_EQUAL (makeWx (ModeConfigWxValues::EnabledWithValidatorAndOneWire).toUint8 (),
					   WX_ENABLED | WX_CHECK_VALIDATE_PARAMS);
}

BOOST_AUTO_TEST_CASE (to_uint8_invalid_mode_throws)
{
	BOOST_CHECK_THROW (makeWx (static_cast<ModeConfigWxValues> (0x55)).toUint8 (),
					   std::out_of_range);
}

// ----------------------------------------------------------------------------
// round trip: text -> internal -> NvMem bitmask -> internal -> text
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE (round_trip_string_uint8_string)
{
	for (const char *str :
		 {"OFF", "ON", "ON_ONEWIRE", "ON_VALIDATOR", "ON_VALIDATOR_ONEWIRE"}) {
		ModeConfigWx fromText (str);
		ModeConfigWx fromRaw (fromText.toUint8 ());
		BOOST_CHECK_EQUAL (fromRaw.toString (), str);
	}
}

BOOST_AUTO_TEST_SUITE_END ()
