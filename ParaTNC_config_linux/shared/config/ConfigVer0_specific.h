/*
 * This file contain decoder and encoders, which converts numeric values of
 * configuration parameters, to something more user friendly. Like a text,
 * which describe what
 *
 * ConfigVer0_specific.h
 *
 *  Created on: Sep 29, 2026
 *      Author: mateusz
 */

#ifndef SHARED_CONFIG_CONFIGVER0_SPECIFIC_H_
#define SHARED_CONFIG_CONFIGVER0_SPECIFIC_H_

#include <cstdint>
#include <iostream>
#include <string>

#include "stored_configuration_nvm/config_data.h"

// ============================================================================
// Specific decoders/encoders to more user-friendy values for PT sensor config
// ============================================================================

enum class BasicConfigPtResistance { OFF = 0U, PT100 = 3U, PT1000 = 1U };

enum class BasicConfigReferenceRes {
	_430 = 0,
	_432 = 1,
	_442 = 2,
	_470 = 3,
	_499 = 4,
	_510 = 5,
	_560 = 6,
	_620 = 7,
	_680 = 8,
	_768 = 9,
	_1000 = 10,
	_1100 = 11,
	_1200 = 12,
	_1300 = 13,
	_1400 = 14,
	_1500 = 15,
	_1600 = 16,
	_1740 = 17,
	_1800 = 18,
	_1910 = 19,
	_2000 = 20,
	_2100 = 21,
	_2400 = 22,
	_2700 = 23,
	_3000 = 24,
	_3090 = 25,
	_3400 = 26,
	_3900 = 27,
	_4300 = 28,
	_4700 = 29,
	_4990 = 30,
	_5600 = 31,

};

struct BasicConfigPtSensor {
	BasicConfigPtResistance type;
	BasicConfigReferenceRes referenceResistor;

	/**
	 * @brief converts value to string to be stored in configuration text file
	 */
	std::string toString ()
	{
		if (type == BasicConfigPtResistance::OFF) {
			return "false";
		}

		std::string typeStr;
		switch (type) {
		case BasicConfigPtResistance::PT100: typeStr = "PT100"; break;
		case BasicConfigPtResistance::PT1000: typeStr = "PT1000"; break;
		default: return "false";
		}

		std::string resistorStr;
		switch (referenceResistor) {
		case BasicConfigReferenceRes::_430: resistorStr = "430"; break;
		case BasicConfigReferenceRes::_432: resistorStr = "432"; break;
		case BasicConfigReferenceRes::_442: resistorStr = "442"; break;
		case BasicConfigReferenceRes::_470: resistorStr = "470"; break;
		case BasicConfigReferenceRes::_499: resistorStr = "499"; break;
		case BasicConfigReferenceRes::_510: resistorStr = "510"; break;
		case BasicConfigReferenceRes::_560: resistorStr = "560"; break;
		case BasicConfigReferenceRes::_620: resistorStr = "620"; break;
		case BasicConfigReferenceRes::_680: resistorStr = "680"; break;
		case BasicConfigReferenceRes::_768: resistorStr = "768"; break;
		case BasicConfigReferenceRes::_1000: resistorStr = "1000"; break;
		case BasicConfigReferenceRes::_1100: resistorStr = "1100"; break;
		case BasicConfigReferenceRes::_1200: resistorStr = "1200"; break;
		case BasicConfigReferenceRes::_1300: resistorStr = "1300"; break;
		case BasicConfigReferenceRes::_1400: resistorStr = "1400"; break;
		case BasicConfigReferenceRes::_1500: resistorStr = "1500"; break;
		case BasicConfigReferenceRes::_1600: resistorStr = "1600"; break;
		case BasicConfigReferenceRes::_1740: resistorStr = "1740"; break;
		case BasicConfigReferenceRes::_1800: resistorStr = "1800"; break;
		case BasicConfigReferenceRes::_1910: resistorStr = "1910"; break;
		case BasicConfigReferenceRes::_2000: resistorStr = "2000"; break;
		case BasicConfigReferenceRes::_2100: resistorStr = "2100"; break;
		case BasicConfigReferenceRes::_2400: resistorStr = "2400"; break;
		case BasicConfigReferenceRes::_2700: resistorStr = "2700"; break;
		case BasicConfigReferenceRes::_3000: resistorStr = "3000"; break;
		case BasicConfigReferenceRes::_3090: resistorStr = "3090"; break;
		case BasicConfigReferenceRes::_3400: resistorStr = "3400"; break;
		case BasicConfigReferenceRes::_3900: resistorStr = "3900"; break;
		case BasicConfigReferenceRes::_4300: resistorStr = "4300"; break;
		case BasicConfigReferenceRes::_4700: resistorStr = "4700"; break;
		case BasicConfigReferenceRes::_4990: resistorStr = "4990"; break;
		case BasicConfigReferenceRes::_5600: resistorStr = "5600"; break;
		default: resistorStr = "0"; break;
		}

		return typeStr + "_" + resistorStr;
	};

	/**
	 * @brief Converts value from a controller config NvMem area, variable 'wx_pt_sensor' to
	 * internal value used by this tool
	 * @note this is a snippet of code from controller embedded code.
	 * value of an enum BasicConfigPtResistance is taken from
	 *      'main_config_data_mode->wx_pt_sensor & 0x3'
	 * value of an enum BasicConfigReferenceRes is taken from
	 *      '(main_config_data_mode->wx_pt_sensor & 0xFC) >> 2)'
	 *
	 * full snippet:
	 *     	max31865_init (main_config_data_mode->wx_pt_sensor & 0x3,
	 *	(main_config_data_mode->wx_pt_sensor & 0xFC) >> 2)
	 *
	 * taken from:   https://github.com/SP8EBC/ParaTNC/blob/master/src/main.c  line 1380
	 *      */
	BasicConfigPtSensor (uint8_t from)
		: type (BasicConfigPtResistance::OFF), referenceResistor (BasicConfigReferenceRes::_4300)
	{
		// 0x00 and 0xFF both mean "PT sensor disabled", regardless of what the
		// lower bits would otherwise decode to
		if (from == 0x00 || from == 0xFF) {
			return;
		}

		uint8_t rawType = from & 0x3;
		uint8_t rawResistor = (from & 0xFC) >> 2;

		BasicConfigPtResistance parsedType;
		switch (rawType) {
		case 0U: parsedType = BasicConfigPtResistance::OFF; break;
		case 1U: parsedType = BasicConfigPtResistance::PT1000; break;
		case 3U: parsedType = BasicConfigPtResistance::PT100; break;
		default:
			std::cout
				<< "W = BasicConfigPtSensor::BasicConfigPtSensor, unknown value of uint8_t from: "
				<< std::hex << from << std::endl;
			// reserved/unknown combination of the type bits - treat as disabled
			return;
		}

		if (rawResistor > static_cast<uint8_t> (BasicConfigReferenceRes::_5600)) {
			std::cout
				<< "W = BasicConfigPtSensor::BasicConfigPtSensor, unknown value of uint8_t from: "
				<< std::hex << from << std::endl;
			std::cout << "W = BasicConfigPtSensor::BasicConfigPtSensor, reference resistor index "
						 "outside of the known lookup table "
					  << std::endl;
			// reference resistor index outside of the known lookup table - treat as disabled
			return;
		}

		type = parsedType;
		referenceResistor = static_cast<BasicConfigReferenceRes> (rawResistor);
	}

	/**
	 * @brief Converts string read from configuration text file to internal value used by this
	 * config tool
	 */
	BasicConfigPtSensor (std::string from)
		: type (BasicConfigPtResistance::OFF), referenceResistor (BasicConfigReferenceRes::_4300)
	{
		std::string typeStr;
		std::string resistorStr;

		std::string upper = from;
		std::transform (upper.begin (), upper.end (), upper.begin (), [] (unsigned char c) {
			return std::toupper (c);
		});

		if (upper == "FALSE") {
			return;
		}

		// values with default reference resistors values
		if (upper == "PT100") {
			typeStr = "PT100";
			resistorStr = "4300";

			std::cout << "W = BasicConfigPtSensor::BasicConfigPtSensor, special value: " << from
					  << " parsed to, typeStr: " << typeStr << ", resistorStr: " << resistorStr
					  << std::endl;
		}
		else if (upper == "PT1000") {
			typeStr = "PT1000";
			resistorStr = "4300";

			std::cout << "W = BasicConfigPtSensor::BasicConfigPtSensor, special value: " << from
					  << " parsed to, typeStr: " << typeStr << ", resistorStr: " << resistorStr
					  << std::endl;
		}
		else {
			size_t separator = upper.find ('_');
			if (separator == std::string::npos) {
				std::cout << "E = BasicConfigPtSensor::BasicConfigPtSensor, malformed value: "
						  << from << std::endl;
				std::cout << "E = BasicConfigPtSensor::BasicConfigPtSensor, PT temperature sensor "
							 "disabled. "
						  << std::endl;
				return;
			}

			typeStr = upper.substr (0, separator);
			resistorStr = upper.substr (separator + 1);
		}

		BasicConfigPtResistance parsedType;
		if (typeStr == "PT100") {
			parsedType = BasicConfigPtResistance::PT100;
		}
		else if (typeStr == "PT1000") {
			parsedType = BasicConfigPtResistance::PT1000;
		}
		else {
			return;
		}

		BasicConfigReferenceRes parsedResistor;
		if (resistorStr == "430")
			parsedResistor = BasicConfigReferenceRes::_430;
		else if (resistorStr == "432")
			parsedResistor = BasicConfigReferenceRes::_432;
		else if (resistorStr == "442")
			parsedResistor = BasicConfigReferenceRes::_442;
		else if (resistorStr == "470")
			parsedResistor = BasicConfigReferenceRes::_470;
		else if (resistorStr == "499")
			parsedResistor = BasicConfigReferenceRes::_499;
		else if (resistorStr == "510")
			parsedResistor = BasicConfigReferenceRes::_510;
		else if (resistorStr == "560")
			parsedResistor = BasicConfigReferenceRes::_560;
		else if (resistorStr == "620")
			parsedResistor = BasicConfigReferenceRes::_620;
		else if (resistorStr == "680")
			parsedResistor = BasicConfigReferenceRes::_680;
		else if (resistorStr == "768")
			parsedResistor = BasicConfigReferenceRes::_768;
		else if (resistorStr == "1000")
			parsedResistor = BasicConfigReferenceRes::_1000;
		else if (resistorStr == "1100")
			parsedResistor = BasicConfigReferenceRes::_1100;
		else if (resistorStr == "1200")
			parsedResistor = BasicConfigReferenceRes::_1200;
		else if (resistorStr == "1300")
			parsedResistor = BasicConfigReferenceRes::_1300;
		else if (resistorStr == "1400")
			parsedResistor = BasicConfigReferenceRes::_1400;
		else if (resistorStr == "1500")
			parsedResistor = BasicConfigReferenceRes::_1500;
		else if (resistorStr == "1600")
			parsedResistor = BasicConfigReferenceRes::_1600;
		else if (resistorStr == "1740")
			parsedResistor = BasicConfigReferenceRes::_1740;
		else if (resistorStr == "1800")
			parsedResistor = BasicConfigReferenceRes::_1800;
		else if (resistorStr == "1910")
			parsedResistor = BasicConfigReferenceRes::_1910;
		else if (resistorStr == "2000")
			parsedResistor = BasicConfigReferenceRes::_2000;
		else if (resistorStr == "2100")
			parsedResistor = BasicConfigReferenceRes::_2100;
		else if (resistorStr == "2400")
			parsedResistor = BasicConfigReferenceRes::_2400;
		else if (resistorStr == "2700")
			parsedResistor = BasicConfigReferenceRes::_2700;
		else if (resistorStr == "3000")
			parsedResistor = BasicConfigReferenceRes::_3000;
		else if (resistorStr == "3090")
			parsedResistor = BasicConfigReferenceRes::_3090;
		else if (resistorStr == "3400")
			parsedResistor = BasicConfigReferenceRes::_3400;
		else if (resistorStr == "3900")
			parsedResistor = BasicConfigReferenceRes::_3900;
		else if (resistorStr == "4300")
			parsedResistor = BasicConfigReferenceRes::_4300;
		else if (resistorStr == "4700")
			parsedResistor = BasicConfigReferenceRes::_4700;
		else if (resistorStr == "4990")
			parsedResistor = BasicConfigReferenceRes::_4990;
		else if (resistorStr == "5600")
			parsedResistor = BasicConfigReferenceRes::_5600;
		else {
			// default reference resistor value
			parsedResistor = BasicConfigReferenceRes::_4300;
			std::cout << "W = BasicConfigPtSensor::BasicConfigPtSensor, default reference resistor "
						 "of 4300 ohms"
					  << std::endl;
		}

		type = parsedType;
		referenceResistor = parsedResistor;
	}

	/**
	 * @brief Converts internal value used by this tool back to a single byte to be stored in
	 * a controller config NvMem area, variable 'wx_pt_sensor'
	 * @note this is the inverse of the bit layout described in the uint8_t constructor above:
	 *      bits 0-1 -> BasicConfigPtResistance (type)
	 *      bits 2-7 -> BasicConfigReferenceRes (referenceResistor)
	 */
	uint8_t toUint8 () const
	{
		if (type == BasicConfigPtResistance::OFF) {
			return 0x00U;
		}

		uint8_t rawType = static_cast<uint8_t> (type) & 0x3U;
		uint8_t rawResistor = static_cast<uint8_t> (referenceResistor) & 0x3FU;

		return static_cast<uint8_t> ((rawResistor << 2) | rawType);
	}
};

// ============================================================================
// Specific decoders/encoders to more user-friendy values for data source
// ============================================================================

enum class SourceConfigForWhat { Wind, Temperature, Pressure, Humidity };

struct SourceConfigSourceConfig {
	config_data_wx_sources_enum_t source; //!< Exact type from embedded SW
	SourceConfigForWhat forWhat;

	/**
	 *
	 * @return
	 */
	std::string toString ()
	{
		std::string out = "";

		switch (source) {
		case WX_SOURCE_INTERNAL:
			if (forWhat == SourceConfigForWhat::Wind) {
				out = "DAVIS";
			}
			else if (forWhat == SourceConfigForWhat::Temperature) {
				out = "1WIRE";
			}
			else if (forWhat == SourceConfigForWhat::Humidity) {
				out = "INTERNAL";
			}
			else if (forWhat == SourceConfigForWhat::Pressure) {
				out = "INTERNAL";
			}
			else {
				throw std::runtime_error ("malformed input");
				out = "???"; // generally this should never happen.
			}
			break;
		case WX_SOURCE_INTERNAL_PT100:
			if (forWhat == SourceConfigForWhat::Temperature) {
				out = "PT1000";
			}
			else {
				out = "????";
				std::cout
					<< "E = SourceConfigSourceConfig::toString, INTERNAL_PT1000 makes sense only"
					<< " for temperature readout!!" << std::endl;
				throw std::runtime_error ("malformed input");
			}
			break;
		case WX_SOURCE_UMB: out = "UMB"; break;
		case WX_SOURCE_RTU: out = "MODBUS"; break;
		case WX_SOURCE_FULL_RTU:
			if (forWhat == SourceConfigForWhat::Wind) {
				out = "MODBUS_FULL";
			}
			else {
				out = "????";
				std::cout
					<< "E = SourceConfigSourceConfig::toString, SOURCE_FULL_RTU makes sense only"
					<< " for wind readout!!" << std::endl;
				throw std::runtime_error ("malformed input");
			}
			break;
		case WX_SOURCE_DAVIS_SERIAL: out = "DAVIS_SERIAL_LOGGER"; break;
		}

		return out;
	}

	/**
	 * @brief Converts value from a controller config NvMem area, field 'config_data_wx_sources_t'
	 * to internal value used by this tool
	 *      */
	SourceConfigSourceConfig (uint8_t from, SourceConfigForWhat what) : forWhat (what)
	{
		// sanitizing user input
		switch (what) {
		case SourceConfigForWhat::Wind:
			if (from == WX_SOURCE_INTERNAL) {
				source = WX_SOURCE_INTERNAL; // string: DAVIS
			}
			else if (from == WX_SOURCE_UMB) {
				source = WX_SOURCE_UMB; // string: UMB
			}
			else if (from == WX_SOURCE_RTU) {
				source = WX_SOURCE_RTU;
			}
			else if (from == WX_SOURCE_FULL_RTU) {
				source = WX_SOURCE_FULL_RTU;
			}
			else if (from == WX_SOURCE_DAVIS_SERIAL) {
				source = WX_SOURCE_DAVIS_SERIAL;
			}
			else {
				std::cout << "E = SourceConfigSourceConfig::SourceConfigSourceConfig, "
							 "what: SourceConfigForWhat::Wind, from: "
						  << (int)from << std::endl;
				throw std::runtime_error ("malformed input");
			}
			break;
		case SourceConfigForWhat::Temperature:
			if (from <= WX_SOURCE_INTERNAL_PT100) {
				source = (config_data_wx_sources_enum_t)from;
			}
			else {
				std::cout << "E = SourceConfigSourceConfig::SourceConfigSourceConfig, "
							 "what: SourceConfigForWhat::Temperature, from: "
						  << (int)from << std::endl;
				throw std::runtime_error ("malformed input");
			}
			break;
		case SourceConfigForWhat::Pressure:
			if (from != WX_SOURCE_INTERNAL_PT100) {
				source = (config_data_wx_sources_enum_t)from;
			}
			else {
				std::cout << "E = SourceConfigSourceConfig::SourceConfigSourceConfig, "
							 "what: SourceConfigForWhat::Pressure, from: "
						  << (int)from << std::endl;
				throw std::runtime_error ("malformed input");
			}
			break;
		case SourceConfigForWhat::Humidity:
			if (from != WX_SOURCE_INTERNAL_PT100) {
				source = (config_data_wx_sources_enum_t)from;
			}
			else {
				std::cout << "E = SourceConfigSourceConfig::SourceConfigSourceConfig, "
							 "what: SourceConfigForWhat::Pressure, from: "
						  << (int)from << std::endl;
				throw std::runtime_error ("malformed input");
			}
			break;
		}
	}

	/**
	 * @brief Converts string read from configuration text file to internal value used by this
	 * config tool
	 */
	SourceConfigSourceConfig (std::string from, SourceConfigForWhat what) : forWhat (what)
	{
		std::string upper = from;
		std::transform (upper.begin (), upper.end (), upper.begin (), [] (unsigned char c) {
			return std::toupper (c);
		});

		if (upper == "DAVIS") {
			source = WX_SOURCE_INTERNAL;
		}
		else if (upper == "1WIRE") {
			source = WX_SOURCE_INTERNAL;
		}
		else if (upper == "PT1000") {
			source = WX_SOURCE_INTERNAL_PT100;
		}
		else if (upper == "INTERNAL") {
			source = WX_SOURCE_INTERNAL;
		}
		else if (upper == "UMB") {
			source = WX_SOURCE_UMB;
		}
		else if (upper == "MODBUS") {
			source = WX_SOURCE_RTU;
		}
		else if (upper == "MODBUS_FULL") {
			source = WX_SOURCE_FULL_RTU;
		}
		else if (upper == "DAVIS_SERIAL_LOGGER") {
			source = WX_SOURCE_DAVIS_SERIAL;
		}
		else {
			std::cout << "E = SourceConfigSourceConfig::SourceConfigSourceConfig, "
						 "from: "
					  << from << std::endl;
			throw std::runtime_error (
				"unknown value 'from' to construct an instance of SourceConfigSourceConfig from!");
		}
	}

	/**
	 * @brief Converts internal value used by this tool back to a single byte to be stored in
	 * a controller configuration NvMem area
	 */
	uint8_t toUint8 () const { return static_cast<uint8_t> (source); }
};

#endif /* SHARED_CONFIG_CONFIGVER0_SPECIFIC_H_ */
