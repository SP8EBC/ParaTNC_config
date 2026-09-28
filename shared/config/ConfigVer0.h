/*
 * ConfigVer0.h
 *
 *  Created on: Nov 18, 2025
 *      Author: mateusz
 */

#ifndef SHARED_CONFIG_CONFIGVER0_H_
#define SHARED_CONFIG_CONFIGVER0_H_

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "IConfig.h"

// ============================================================================
// BASIC Configuration Decoder/Encoder
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
	 * @brief Converts value from a controller configuration binary blob 'wx_pt_sensor' to
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

			std::cout <<    
                        "W = BasicConfigPtSensor::BasicConfigPtSensor, special value: " << from <<
                        " parsed to, typeStr: " << typeStr << ", resistorStr: " << resistorStr
			<< std::endl;
        }
        else if (upper == "PT1000") {
            typeStr = "PT1000";
            resistorStr = "4300";
            
            std::cout <<    
                        "W = BasicConfigPtSensor::BasicConfigPtSensor, special value: " << from <<
                        " parsed to, typeStr: " << typeStr << ", resistorStr: " << resistorStr
			<< std::endl;
        }
        else {
            size_t separator = upper.find ('_');
            if (separator == std::string::npos) {
                std::cout <<    
                            "E = BasicConfigPtSensor::BasicConfigPtSensor, malformed value: " << from << std::endl;
                std::cout <<    
                            "E = BasicConfigPtSensor::BasicConfigPtSensor, PT temperature sensor disabled. " << std::endl;
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
	 * a controller configuration binary blob 'wx_pt_sensor'
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

class BasicConfig : public IBasicConfig {
  protected:
	const std::vector<uint8_t> &configData;

	// Helper methods
	std::string readString (size_t offset, size_t maxLength) const;
	void writeString (size_t offset, size_t maxLength, const std::string &value);

	template <typename T> T readValue (size_t offset) const
	{
		if (offset + sizeof (T) > configData.size ()) {
			throw std::out_of_range ("Config data access out of bounds");
		}
		T value;
		std::memcpy (&value, &configData[offset], sizeof (T));
		return value;
	}

	template <typename T> void writeValue (size_t offset, T value)
	{
		if (offset + sizeof (T) > configData.size ()) {
			throw std::out_of_range ("Config data access out of bounds");
		}
		std::memcpy (&const_cast<std::vector<uint8_t> &> (configData)[offset], &value, sizeof (T));
	}

  public:
	BasicConfig (const std::vector<uint8_t> &data);
	virtual ~BasicConfig () = default;

	// Decoder methods
	virtual void getCallsign (std::string &call) const;
	virtual std::string getCallsign () const;
	virtual uint8_t getSsid () const;
	virtual float getLatitude () const;
	virtual uint8_t getNs () const;
	virtual float getLongitude () const;
	virtual uint8_t getWe () const;
	virtual void getComment (std::string &comment) const;
	virtual std::string getComment () const;
	virtual uint8_t getSymbol () const;
	virtual uint8_t getPathType () const;
	virtual bool getBeaconBootup () const;
	virtual uint8_t getWxTransmitPeriod () const;
	virtual uint8_t getBeaconTransmitPeriod () const;
	virtual bool getWxDoubleTransmit () const;

	// Encoder methods
	virtual void setCallsign (const std::string &call);
	virtual void setSsid (uint8_t ssid);
	virtual void setLatitude (float latitude);
	virtual void setNs (uint8_t ns);
	virtual void setLongitude (float longitude);
	virtual void setWe (uint8_t we);
	virtual void setComment (const std::string &comment);
	virtual void setSymbol (uint8_t symbol);
	virtual void setPathType (uint8_t pathType);
	virtual void setBeaconBootup (bool bootup);
	virtual void setWxTransmitPeriod (uint8_t period);
	virtual void setBeaconTransmitPeriod (uint8_t period);
	virtual void setWxDoubleTransmit (bool doubleTransmit);
};

// ============================================================================
// MODE Configuration Decoder/Encoder
// ============================================================================
class ModeConfig : public IModeConfig {
  protected:
	const std::vector<uint8_t> &configData;

	template <typename T> T readValue (size_t offset) const
	{
		if (offset + sizeof (T) > configData.size ()) {
			throw std::out_of_range ("Config data access out of bounds");
		}
		T value;
		std::memcpy (&value, &configData[offset], sizeof (T));
		return value;
	}

	template <typename T> void writeValue (size_t offset, T value)
	{
		if (offset + sizeof (T) > configData.size ()) {
			throw std::out_of_range ("Config data access out of bounds");
		}
		std::memcpy (&const_cast<std::vector<uint8_t> &> (configData)[offset], &value, sizeof (T));
	}

  public:
	ModeConfig (const std::vector<uint8_t> &data);
	virtual ~ModeConfig () = default;

	virtual uint8_t getDigi () const;
	virtual uint8_t getWx () const;
	virtual bool getWxUmb () const;
	virtual bool getWxModbus () const;
	virtual bool getWxDavis () const;
	virtual bool getWxMs5611OrBme () const;
	virtual uint8_t getWxAnemometerConst () const;
	virtual uint8_t getWxDustSensor () const;
	virtual uint8_t getWxPtSensor () const;
	virtual bool getVictron () const;
	virtual bool getDigiViscous () const;
	virtual bool getDigiOnlySsid () const;
	virtual uint8_t getDigiViscousDelay () const;
	virtual uint8_t getDigiDelay100msec () const;
	virtual uint8_t getPowersave () const;
	virtual bool getPowersaveKeepGsm () const;
	virtual bool getGsm () const;

	virtual void setDigi (uint8_t digi);
	virtual void setWx (uint8_t wx);
	virtual void setWxUmb (bool wxUmb);
	virtual void setWxModbus (bool wxModbus);
	virtual void setWxDavis (bool wxDavis);
	virtual void setWxMs5611OrBme (bool sensor);
	virtual void setWxAnemometerConst (uint8_t anemometer);
	virtual void setWxDustSensor (uint8_t dust);
	virtual void setWxPtSensor (uint8_t ptSensor);
	virtual void setVictron (bool victron);
	virtual void setDigiViscous (bool viscous);
	virtual void setDigiOnlySsid (bool onlySsid);
	virtual void setDigiViscousDelay (uint8_t delay);
	virtual void setDigiDelay100msec (uint8_t delay);
	virtual void setPowersave (uint8_t powersave);
	virtual void setPowersaveKeepGsm (bool keepGsm);
	virtual void setGsm (bool gsm);
};

// ============================================================================
// SOURCE Configuration Decoder/Encoder
// ============================================================================
class SourceConfig : public ISourceConfig {
  protected:
	const std::vector<uint8_t> &configData;

	template <typename T> T readValue (size_t offset) const
	{
		if (offset + sizeof (T) > configData.size ()) {
			throw std::out_of_range ("Config data access out of bounds");
		}
		T value;
		std::memcpy (&value, &configData[offset], sizeof (T));
		return value;
	}

	template <typename T> void writeValue (size_t offset, T value)
	{
		if (offset + sizeof (T) > configData.size ()) {
			throw std::out_of_range ("Config data access out of bounds");
		}
		std::memcpy (&const_cast<std::vector<uint8_t> &> (configData)[offset], &value, sizeof (T));
	}

  public:
	SourceConfig (const std::vector<uint8_t> &data);
	virtual ~SourceConfig () = default;

	virtual uint8_t getTemperature () const;
	virtual uint8_t getPressure () const;
	virtual uint8_t getHumidity () const;
	virtual uint8_t getWind () const;

	virtual void setTemperature (uint8_t temp);
	virtual void setPressure (uint8_t pressure);
	virtual void setHumidity (uint8_t humidity);
	virtual void setWind (uint8_t wind);
};

// ============================================================================
// UMB Configuration Decoder/Encoder
// ============================================================================
class UmbConfig : public IUmbConfig {
  protected:
	const std::vector<uint8_t> &configData;

	template <typename T> T readValue (size_t offset) const
	{
		if (offset + sizeof (T) > configData.size ()) {
			throw std::out_of_range ("Config data access out of bounds");
		}
		T value;
		std::memcpy (&value, &configData[offset], sizeof (T));
		return value;
	}

	template <typename T> void writeValue (size_t offset, T value)
	{
		if (offset + sizeof (T) > configData.size ()) {
			throw std::out_of_range ("Config data access out of bounds");
		}
		std::memcpy (&const_cast<std::vector<uint8_t> &> (configData)[offset], &value, sizeof (T));
	}

  public:
	UmbConfig (const std::vector<uint8_t> &data);
	virtual ~UmbConfig () = default;

	virtual uint16_t getSlaveClass () const;
	virtual uint16_t getSlaveId () const;
	virtual uint16_t getChannelWindSpeed () const;
	virtual uint16_t getChannelWindGust () const;
	virtual uint16_t getChannelWindDirection () const;
	virtual uint16_t getChannelTemperature () const;
	virtual uint16_t getChannelQnh () const;

	virtual void setSlaveClass (uint16_t slaveClass);
	virtual void setSlaveId (uint16_t slaveId);
	virtual void setChannelWindSpeed (uint16_t channel);
	virtual void setChannelWindGust (uint16_t channel);
	virtual void setChannelWindDirection (uint16_t channel);
	virtual void setChannelTemperature (uint16_t channel);
	virtual void setChannelQnh (uint16_t channel);
};

// ============================================================================
// RTU Configuration Decoder/Encoder
// ============================================================================
class RtuConfig : public IRtuConfig {
  protected:
	const std::vector<uint8_t> &configData;

	template <typename T> T readValue (size_t offset) const
	{
		if (offset + sizeof (T) > configData.size ()) {
			throw std::out_of_range ("Config data access out of bounds");
		}
		T value;
		std::memcpy (&value, &configData[offset], sizeof (T));
		return value;
	}

	template <typename T> void writeValue (size_t offset, T value)
	{
		if (offset + sizeof (T) > configData.size ()) {
			throw std::out_of_range ("Config data access out of bounds");
		}
		std::memcpy (&const_cast<std::vector<uint8_t> &> (configData)[offset], &value, sizeof (T));
	}

  public:
	RtuConfig (const std::vector<uint8_t> &data);
	virtual ~RtuConfig () = default;

	virtual uint16_t getSlaveSpeed () const;
	virtual uint8_t getSlaveParity () const;
	virtual uint8_t getSlaveStopBits () const;
	virtual uint8_t getUseFullWindData () const;
	virtual uint8_t getTemperatureSrc () const;
	virtual uint8_t getHumiditySrc () const;
	virtual uint8_t getPressureSrc () const;
	virtual uint8_t getWindDir () const;
	virtual uint8_t getWindSpeed () const;
	virtual uint8_t getWindGusts () const;
	virtual RtuSlave getSlave (uint8_t id) const override;

	virtual void setSlaveSpeed (uint16_t speed);
	virtual void setSlaveParity (uint8_t parity);
	virtual void setSlaveStopBits (uint8_t stopBits);
	virtual void setUseFullWindData (uint8_t useFullWind);
	virtual void setTemperatureSrc (uint8_t tempSrc);
	virtual void setHumiditySrc (uint8_t humiditySrc);
	virtual void setPressureSrc (uint8_t pressureSrc);
	virtual void setWindDir (uint8_t windDir);
	virtual void setWindSpeed (uint8_t windSpeed);
	virtual void setWindGusts (uint8_t windGusts);
	virtual void setSlave (uint8_t id, RtuSlave &data) override;
	virtual size_t howManySlaves () const;
};

// ============================================================================
// GSM Configuration Decoder/Encoder
// ============================================================================
class GsmConfig : public IGsmConfig {
  protected:
	const std::vector<uint8_t> &configData;

	std::string readString (size_t offset, size_t maxLength) const;
	void writeString (size_t offset, size_t maxLength, const std::string &value);

	template <typename T> T readValue (size_t offset) const
	{
		if (offset + sizeof (T) > configData.size ()) {
			throw std::out_of_range ("Config data access out of bounds");
		}
		T value;
		std::memcpy (&value, &configData[offset], sizeof (T));
		return value;
	}

	template <typename T> void writeValue (size_t offset, T value)
	{
		if (offset + sizeof (T) > configData.size ()) {
			throw std::out_of_range ("Config data access out of bounds");
		}
		std::memcpy (&const_cast<std::vector<uint8_t> &> (configData)[offset], &value, sizeof (T));
	}

  public:
	GsmConfig (const std::vector<uint8_t> &data);
	virtual ~GsmConfig () = default;

	virtual void getPin (std::string &pin) const;
	virtual std::string getPin () const;
	virtual void getApn (std::string &apn) const;
	virtual std::string getApn () const;
	virtual void getUsername (std::string &username) const;
	virtual std::string getUsername () const;
	virtual void getPassword (std::string &password) const;
	virtual std::string getPassword () const;
	virtual uint8_t getApiEnable () const;
	virtual void getApiBaseUrl (std::string &url) const;
	virtual std::string getApiBaseUrl () const;
	virtual void getApiStationName (std::string &stationName) const;
	virtual std::string getApiStationName () const;
	virtual uint8_t getAprsisEnable () const;
	virtual void getAprsisServer (std::string &server) const;
	virtual std::string getAprsisServer () const;
	virtual uint16_t getAprsisPort () const;
	virtual void getAprsisPpasscode (std::string &passcode) const;
	virtual std::string getAprsisPpasscode () const;

	virtual void setPin (const std::string &pin);
	virtual void setApn (const std::string &apn);
	virtual void setUsername (const std::string &username);
	virtual void setPassword (const std::string &password);
	virtual void setApiEnable (uint8_t apiEnable);
	virtual void setApiBaseUrl (const std::string &url);
	virtual void setApiStationName (const std::string &stationName);
	virtual void setAprsisEnable (uint8_t aprsisEnable);
	virtual void setAprsisServer (const std::string &server);
	virtual void setAprsisPort (uint16_t port);
	virtual void setAprsisPpasscode (const std::string &passcode);
};

// ============================================================================
// Main Configuration Manager
// ============================================================================
class ConfigurationManager : public IConfigurationManager {
  private:
	std::vector<uint8_t> configData;
	BasicConfig basicConfig;
	ModeConfig modeConfig;
	SourceConfig sourceConfig;
	UmbConfig umbConfig;
	RtuConfig rtuConfig;
	GsmConfig gsmConfig;

  public:
	ConfigurationManager (const std::vector<uint8_t> &data)
		: configData (data), basicConfig (configData), modeConfig (configData),
		  sourceConfig (configData), umbConfig (configData), rtuConfig (configData),
		  gsmConfig (configData)
	{
	}

	ConfigurationManager ();

	virtual IBasicConfig &getBasicConfig () { return basicConfig; }
	virtual IModeConfig &getModeConfig () { return modeConfig; }
	virtual ISourceConfig &getSourceConfig () { return sourceConfig; }
	virtual IUmbConfig &getUmbConfig () { return umbConfig; }
	virtual IRtuConfig &getRtuConfig () { return rtuConfig; }
	virtual IGsmConfig &getGsmConfig () { return gsmConfig; }

	virtual uint32_t getConfigCounter ();
	virtual void setConfigCounter (uint32_t value);

	virtual uint32_t calculateAndSetChecksum ();

	const std::vector<uint8_t> &getConfigData () const { return configData; }

	virtual void print (PrintVerbosity verbosity);
};
#endif /* SHARED_CONFIG_CONFIGVER0_H_ */
