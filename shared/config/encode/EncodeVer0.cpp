/*
 * EncodeVer0.cpp
 *
 *  Created on: Apr 23, 2023
 *      Author: mateusz
 */

#include "stdafx.h"

#include "EncodeVer0.h"

#include "../Ver0Map.h"
#include "../../exceptions/NotSupportedEx.h"

#include <string.h>
#include <stdexcept>

EncodeVer0::EncodeVer0(std::vector<uint8_t> & in ) : data(in){

	if (in.size() == 0)
	{
		this->data.reserve(CONFIG__END__OFFSET - 1);

		// prefill vector with dummy data
		this->data.insert(this->data.begin(), CONFIG__END__OFFSET, 0xAA);

		// set programming counter to initial value
		data[CONFIG_MODE_PGM_CNTR] 		= 0;
		data[CONFIG_MODE_PGM_CNTR + 1] 	= 0;
		data[CONFIG_MODE_PGM_CNTR + 2] 	= 0;
		data[CONFIG_MODE_PGM_CNTR + 3] 	= 0;
	}
}

EncodeVer0::~EncodeVer0() {
}

uint32_t EncodeVer0::incrementProgrammingCounter() {
	uint8_t lsb_byte = data.at(CONFIG_MODE_PGM_CNTR);
	uint8_t nd_byte = data.at(CONFIG_MODE_PGM_CNTR + 1);
	uint8_t rd_byte = data.at(CONFIG_MODE_PGM_CNTR + 2);
	uint8_t msb_byte = data.at(CONFIG_MODE_PGM_CNTR + 3);

	uint32_t current_counter = lsb_byte | (nd_byte << 8) | (rd_byte << 16) | (msb_byte << 24);

	current_counter += 2;

	data[CONFIG_MODE_PGM_CNTR] 		= static_cast<uint8_t>(current_counter 		& 0x000000FFu);
	data[CONFIG_MODE_PGM_CNTR + 1] 	= static_cast<uint8_t>((current_counter 	& 0x0000FF00u) >> 8);
	data[CONFIG_MODE_PGM_CNTR + 2] 	= static_cast<uint8_t>((current_counter 	& 0x00FF0000u) >> 16);
	data[CONFIG_MODE_PGM_CNTR + 3] 	= static_cast<uint8_t>((current_counter 	& 0xFF000000u) >> 24);

	return current_counter;
}

void EncodeVer0::setEorW(bool _eOrW) {
	if (_eOrW) {
		data[CONFIG_BASIC_OFFSET + BASIC_WE_OFFSET] = 1;
	}
	else {
		data[CONFIG_BASIC_OFFSET + BASIC_WE_OFFSET] = 0;
	}
}

void EncodeVer0::setRtuSlaveSpeed(uint16_t _slaveSpeedBps) {
	uint8_t lsb_byte = _slaveSpeedBps & 0xFFU;
	uint8_t msb_byte = (_slaveSpeedBps & 0xFF00U) >> 8;

	data[CONFIG_RTU_OFFSET + RTU_SLAVE_SPEED_OFFSET ] = lsb_byte;
	data[CONFIG_RTU_OFFSET + RTU_SLAVE_SPEED_OFFSET + 1] = msb_byte;
}

void EncodeVer0::setGsmAprsisEnable(bool _apsisEnable) {
	if (_apsisEnable) {
		data[CONFIG_GSM_OFFSET + GSM_APRSIS_ENABLE ] = 1;
	}
	else {
		data[CONFIG_GSM_OFFSET + GSM_APRSIS_ENABLE ] = 0;
	}
}

void EncodeVer0::setCallsign(const std::string &call) {
	std::size_t offset = BASIC_CALLSIGN_OFFSET + CONFIG_BASIC_OFFSET;

	memcpy(&data[offset], call.c_str(), call.size());
}

void EncodeVer0::setUmbChannelWinddirection(uint16_t _channelDirection) {
	uint8_t lsb_byte = _channelDirection & 0xFFU;
	uint8_t msb_byte = (_channelDirection & 0xFF00U) >> 8;

	data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDDIRECTION ) = lsb_byte;
	data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDDIRECTION + 1) = msb_byte;
}

void EncodeVer0::setAnemometerPulsesConstant(uint8_t constant) {
	data.at(CONFIG_MODE_OFSET + MODE_WX_ANEMOMETER_CONST_OFFSET ) = constant;
}

void EncodeVer0::setPowersave(Powersave _powersave) {

	uint8_t _data = 0;

	switch (_powersave) {
	case PWSAVE_NORMAL:		_data = 1; break;
	case PWSAVE_AGGRESV:	_data = 2; break;
	case PWSAVE_NONE: 		break;
	}

	data[CONFIG_MODE_OFSET + MODE_POWERSAVE_OFFSET] = _data;
}

void EncodeVer0::setGsmApnUsername(const std::string &username) {

	if (username.length() < GSM_USERNAME_LEN) {
		const char * pStr = username.c_str();
		char * pDestination = reinterpret_cast<char*>(&data.at(0)) + GSM_USERNAME_OFFSET + CONFIG_GSM_OFFSET;

		memset(pDestination, 0x00, GSM_USERNAME_LEN);
		strncpy(pDestination, pStr, username.length());
	}
	else {
		throw std::runtime_error("Provided APN username is too long");
	}
}

void EncodeVer0::setRtuConfiguredSourceQnh(uint8_t _rtuSrcQnhPressure) {
	data[CONFIG_RTU_OFFSET + RTU_PRESSURE_SRC_OFFSET] = _rtuSrcQnhPressure;
}

void EncodeVer0::setRtuSourceConfiguration(const Rtu &config, uint8_t sourceId) {
	std::size_t channelOffset = CONFIG_RTU_OFFSET + RTU_SLAVE_CONFIG_BLOCK_OFFSET + (RTU_SLAVE_CONFIG_BLOCK_SIZE * sourceId);

	data[RTU_X_FUNCTION + channelOffset] 	= config.function;
	data[RTU_X_SCALLING_A + channelOffset] 	= config.coeff_a;
	data[RTU_X_SCALLING_B + channelOffset]	= config.coeff_b;
	data[RTU_X_SCALLING_C + channelOffset]  = config.coeff_c;
	data[RTU_X_SCALLING_D + channelOffset]  = config.coeff_d;

	data[RTU_X_LENGHT + channelOffset]		= static_cast<uint8_t>(config.lenght & 0xFFu);
	data[RTU_X_LENGHT + channelOffset + 1] 	= static_cast<uint8_t>((config.lenght & 0xFF00u) >> 8);

	data[RTU_X_REGUSTER_ADDR + channelOffset] 		= static_cast<uint8_t>(config.registerAddress & 0xFFu);
	data[RTU_X_REGUSTER_ADDR + channelOffset + 1]	= static_cast<uint8_t>((config.registerAddress & 0xFF00u) >> 8);
}

void EncodeVer0::setWxDoubleTransmit(bool _doubleTransmit) {

	if (_doubleTransmit) {
		data[CONFIG_BASIC_OFFSET + BASIC_WX_DOUBLE_TRANSMIT] = 1;
	}
	else {
		data[CONFIG_BASIC_OFFSET + BASIC_WX_DOUBLE_TRANSMIT] = 0;
	}
}

void EncodeVer0::setRtuSlaveParity(uint8_t _slaveParity) {
	data[CONFIG_MODE_OFSET + MODE_WX_ANEMOMETER_CONST_OFFSET] = _slaveParity;
}

void EncodeVer0::setWindSrc(WeatherSource _windSource) {

	uint8_t byte = 0;

	switch (_windSource) {
		case WX_SOURCE_UMB:
			byte = 2;
			break;
		case WX_SOURCE_RTU:
			byte = 3;
			break;
		case WX_SOURCE_FULL_RTU:
			byte = 4;
			break;
		case WX_SOURCE_DAVIS_SERIAL:
			byte = 5;
			break;
		case WX_SOURCE_INTERNAL:
			byte = 1;
			break;
		case WX_SOURCE_INTERNAL_PT100:
			byte = 6;
			break;
	}

	data[CONFIG_SOURCES_OFFSET + SOURCE_WIND_OFFSET] = byte;
}

void EncodeVer0::setPath(const AprsPath _path) {

	uint8_t byte = 0;

	switch (_path) {
		case PATH_WIDE11_WIDE21:
			throw NotSupportedEx(); // TODO: not yet available
			//byte = 3;
			break;
		case PATH_WIDE11:
			byte = 1;
			break;
		case PATH_WIDE21:
			byte = 2;
			break;
		case PATH_WIDE22:
			throw NotSupportedEx(); // TODO: not yet available
			//byte = 4;
			break;
		case PATH_NOPATH:
			byte = 0;
			break;
	}

	data[CONFIG_BASIC_OFFSET + BASIC_PATHTYPE_OFFSET] = byte;
}

void EncodeVer0::setUmbChannelWindspeed(uint16_t _channelWindspeed) {
	data[CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDSPEED] = static_cast<uint8_t>(_channelWindspeed & 0xFFu);
	data[CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDSPEED + 1] = static_cast<uint8_t>((_channelWindspeed & 0xFF00u) >> 8);

}

void EncodeVer0::setDigiRadiusFilter(uint8_t _radius) {
	throw NotSupportedEx(); // TODO: not yet available

}

void EncodeVer0::setMs5611orBmeSensor(bool _sensor) {

	if (_sensor) {
		data[CONFIG_MODE_OFSET + MODE_WX_MS5611_OR_BME_OFFSET ] = 1;
	}
	else {
		data[CONFIG_MODE_OFSET + MODE_WX_MS5611_OR_BME_OFFSET ] = 0;
	}
}

void EncodeVer0::setBeaconAtStartup(bool _beaconAtStartup) {
	if (_beaconAtStartup) {
		data[CONFIG_BASIC_OFFSET + BASIC_BEACON_BOOTUP_OFFSET] = 1;
	}
	else {
		data[CONFIG_BASIC_OFFSET + BASIC_BEACON_BOOTUP_OFFSET] = 0;
	}
}

void EncodeVer0::setWxEnabled(bool _enabled) {
	if (_enabled) {
		data[CONFIG_MODE_OFSET + MODE_WX_OFFSET] = 1;
	}
	else {
		data[CONFIG_MODE_OFSET + MODE_WX_OFFSET] = 0;
	}
}

void EncodeVer0::setGsmEnabled(bool _enabled) {
	if (_enabled) {
		data[CONFIG_MODE_OFSET + MODE_GSM_OFFSET] = 1;
	}
	else {
		data[CONFIG_MODE_OFSET + MODE_GSM_OFFSET] = 0;
	}
}

void EncodeVer0::setWxUmbEnabled(bool _enabled) {
	if (_enabled) {
		data[CONFIG_MODE_OFSET + MODE_WX_UMB_OFFSET] = 1;
	}
	else {
		data[CONFIG_MODE_OFSET + MODE_WX_UMB_OFFSET] = 0;
	}
}

void EncodeVer0::setVictronEnabled(bool _enabled) {
	if (_enabled) {
		data[CONFIG_MODE_OFSET + MODE_VICTRON_OFFSET] = 1;
	}
	else {
		data[CONFIG_MODE_OFSET + MODE_VICTRON_OFFSET] = 0;
	}
}

void EncodeVer0::setButtonOneFunction(ButtonFunction _buttonOne) {
	throw NotSupportedEx();	// TODO:
}

void EncodeVer0::setUmbSlaveClass(uint16_t _umbSlaveClass) {
	data[CONFIG_UMB_OFFSET + UMB_SLAVE_CLASS_OFFSET] = static_cast<uint8_t>(_umbSlaveClass & 0xFFu);
	data[CONFIG_UMB_OFFSET + UMB_SLAVE_CLASS_OFFSET + 1] = static_cast<uint8_t>((_umbSlaveClass & 0xFF00u) >> 8);

}

void EncodeVer0::setKeepGsmAlwaysOn(bool _alwaysOn) {
	if (_alwaysOn) {
		data[CONFIG_MODE_OFSET + MODE_POWERSAVE_KEEP_GSM_OFFSET] = 1;
	}
	else {
		data[CONFIG_MODE_OFSET + MODE_POWERSAVE_KEEP_GSM_OFFSET] = 0;
	}
}

void EncodeVer0::setDigiFilter(DigiFilter _filter) {
	throw NotSupportedEx();	// TODO: not yet available

}

void EncodeVer0::setRtuSlaveStopBits(uint8_t _slaveStopBits) {
	data[CONFIG_RTU_OFFSET + RTU_SLAVE_STOP_BITS_OFFSET] = _slaveStopBits;

}

void EncodeVer0::setRtuFullWindData(bool _fullWindData) {
	if (_fullWindData) {
		data[CONFIG_RTU_OFFSET + RTU_USE_FULL_WIND_DATA_OFFSET] = 1;
	}
	else {
		data[CONFIG_RTU_OFFSET + RTU_USE_FULL_WIND_DATA_OFFSET] = 0;
	}
}

void EncodeVer0::setDigiEnabled(Digi _digi) {

	uint8_t digi_viscous = 0;

	uint8_t digi = 0;

	switch (_digi) {
	case DIGI_WIDE1:
		digi = 0;
		digi_viscous = 0;
		break;
	case DIGI_VISCOUS_WIDE1: break;
		digi = 1;
		digi_viscous = 1;
		break;
	case DIGI_OFF:
		digi = 0;
		digi_viscous = 0;
		break;
	}

	data[CONFIG_MODE_OFSET + MODE_DIGI_OFFSET] = digi;
	data[CONFIG_MODE_OFSET + MODE_DIGI_VISCOUS_OFFSET] = digi_viscous;
}

void EncodeVer0::setWxDavisEnabled(bool _enabled) {
	if (_enabled) {
		data[CONFIG_MODE_OFSET + MODE_WX_DAVIS_OFFSET] = 1;
	}
	else {
		data[CONFIG_MODE_OFSET + MODE_WX_DAVIS_OFFSET] = 0;
	}
}

void EncodeVer0::setUmbChannelWinggusts(uint16_t _channelWindgst) {
	data[CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDGUST] = static_cast<uint8_t>(_channelWindgst & 0xFFu);
	data[CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDGUST + 1] = static_cast<uint8_t>((_channelWindgst & 0xFF00u) >> 8);

}

void EncodeVer0::setUmbChannelTemperature(uint16_t _channelTemperature) {
	data[CONFIG_UMB_OFFSET + UMB_CHANNEL_TEMPERATURE] = static_cast<uint8_t>(_channelTemperature & 0xFFu);
	data[CONFIG_UMB_OFFSET + UMB_CHANNEL_TEMPERATURE + 1] = static_cast<uint8_t>((_channelTemperature & 0xFF00u) >> 8);

}

void EncodeVer0::setTemperatureSrc(WeatherSource _temperatureSource) {

	uint8_t byte = 0;

	switch(_temperatureSource) {
		case WX_SOURCE_UMB:
			byte = 2;
			break;
		case WX_SOURCE_RTU:
			byte = 3;
			break;
		case WX_SOURCE_FULL_RTU:
			byte = 4;
			break;
		case WX_SOURCE_DAVIS_SERIAL:
			byte = 5;
			break;
		case WX_SOURCE_INTERNAL:
			byte = 1;
			break;
		case WX_SOURCE_INTERNAL_PT100:
			throw NotSupportedEx();	// TODO: not yet available
			break;
	}

	data[CONFIG_SOURCES_OFFSET + SOURCE_TEMPERATURE_OFFSET] = byte;
}

void EncodeVer0::setLatitude(float _latitude) {

	void * ptr = &_latitude;
	uint32_t * uptr = (uint32_t*)ptr;

	uint32_t intermediate = *uptr;
	// VS2005 is complaining about that
	//uint32_t intermediate = reinterpret_cast<uint32_t>(*reinterpret_cast<uint32_t*>(ptr));

	data[CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET] 		= static_cast<uint8_t>(intermediate 	& 0x000000FFu);
	data[CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET + 1] 	= static_cast<uint8_t>((intermediate 	& 0x0000FF00u) >> 8);
	data[CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET + 2] 	= static_cast<uint8_t>((intermediate 	& 0x00FF0000u) >> 16);
	data[CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET + 3] 	= static_cast<uint8_t>((intermediate 	& 0xFF000000u) >> 24);

}

void EncodeVer0::setWxModbusEnabled(bool _enabled) {
	if (_enabled) {
		data[CONFIG_MODE_OFSET + MODE_WX_MODBUS_OFFSET] = 1;
	}
	else {
		data[CONFIG_MODE_OFSET + MODE_WX_MODBUS_OFFSET] = 0;
	}
}

void EncodeVer0::setGsmApnName(const std::string &apn) {

	if (apn.length() < GSM_APN_LENGHT) {
		const char * pStr = apn.c_str();
		char * pDestination = reinterpret_cast<char*>(&data.at(0)) + GSM_APN_OFFSET + CONFIG_GSM_OFFSET;

		memset(pDestination, 0x00, GSM_APN_LENGHT);
		strncpy(pDestination, pStr, apn.length());
	}
	else {
		throw std::runtime_error("Provided APN is too long");
	}

}

void EncodeVer0::setRtuConfiguredSourceWindgusts(uint8_t _rtuSrcWidgusts) {
	data[CONFIG_RTU_OFFSET + RTU_WIND_GUSTS_OFFSET ] = _rtuSrcWidgusts;

}

void EncodeVer0::setVisvousDelayInSec(uint8_t _seconds) {
	data[CONFIG_RTU_OFFSET + RTU_WIND_GUSTS_OFFSET ] = _seconds;
}

void EncodeVer0::setDigiAlwaysMessageAndStatus(bool _alwaysMessageAndSts) {
	throw NotSupportedEx();	// TODO:
}

void EncodeVer0::setGsmPin(uint32_t _pin) {
	data[CONFIG_GSM_OFFSET + GSM_PIN_OFFSET] 		= static_cast<uint8_t>(_pin 	& 0x000000FFu);
	data[CONFIG_GSM_OFFSET + GSM_PIN_OFFSET + 1] 	= static_cast<uint8_t>((_pin 	& 0x0000FF00u) >> 8);
	data[CONFIG_GSM_OFFSET + GSM_PIN_OFFSET + 2] 	= static_cast<uint8_t>((_pin 	& 0x00FF0000u) >> 16);
	data[CONFIG_GSM_OFFSET + GSM_PIN_OFFSET + 3] 	= static_cast<uint8_t>((_pin 	& 0xFF000000u) >> 24);

}

void EncodeVer0::setPressureSrc(WeatherSource _pressureSource) {

	uint8_t byte = 0;

	switch(_pressureSource) {
		case WX_SOURCE_UMB:
			byte = 2;
			break;
		case WX_SOURCE_RTU:
			byte = 3;
			break;
		case WX_SOURCE_FULL_RTU:
			byte = 4;
			break;
		case WX_SOURCE_DAVIS_SERIAL:
			byte = 5;
			break;
		case WX_SOURCE_INTERNAL:
			byte = 1;
			break;
		case WX_SOURCE_INTERNAL_PT100:
			throw NotSupportedEx();	// TODO: not yet available
			break;
	}

	data[CONFIG_SOURCES_OFFSET + SOURCE_PRESSURE_OFFSET] = byte;
}

void EncodeVer0::setDescritpion(const std::string &description) {

	if (description.length() < BASIC_COMMENT_LENGHT) {
		const char * pStr = description.c_str();
		char * pDestination = reinterpret_cast<char*>(&data.at(0)) + BASIC_COMMENT_OFFSET + CONFIG_BASIC_OFFSET;

		memset(pDestination, 0x00, BASIC_COMMENT_LENGHT);
		strncpy(pDestination, pStr, description.length());
	}
	else {
		throw std::runtime_error("Provided beacon description is too long");
	}
}

void EncodeVer0::setGsmApnPassword(const std::string &password) {
	if (password.length() < GSM_USERNAME_LEN) {
		const char * pStr = password.c_str();
		char * pDestination = reinterpret_cast<char*>(&data.at(0)) + GSM_PASSWORD_OFFSET + CONFIG_GSM_OFFSET;

		memset(pDestination, 0x00, GSM_PASSWORD_LEN);
		strncpy(pDestination, pStr, password.length());
	}
	else {
		throw std::runtime_error("Provided APN is too long");
	}
}

void EncodeVer0::setDigiFilterPrefixList(uint8_t entry, std::string &_value) {
	throw NotSupportedEx();	// TODO: not yet available

}

void EncodeVer0::setGsmApiEnable(bool _apiEnable) {
	if (_apiEnable) {
		data[CONFIG_GSM_OFFSET + GSM_API_ENABLE_OFFSET] = 1;
	}
	else {
		data[CONFIG_GSM_OFFSET + GSM_API_ENABLE_OFFSET] = 0;
	}
}

void EncodeVer0::setRtuConfiguredSourceTemperature(uint8_t _rtuSrcTemperature) {
	data[CONFIG_RTU_OFFSET + RTU_TEMPERATURE_SRC_OFFSET ] = _rtuSrcTemperature;

}

void EncodeVer0::setNorS(bool _nOrS) {
	if (_nOrS) {
		data[CONFIG_BASIC_OFFSET + BASIC_NS_OFFSET] = 1;
	}
	else {
		data[CONFIG_BASIC_OFFSET + BASIC_NS_OFFSET] = 0;
	}
}

void EncodeVer0::setUmbSlaveId(uint16_t _slaveId) {
	data[CONFIG_UMB_OFFSET + UMB_SLAVE_ID_OFFSET] = static_cast<uint8_t>(_slaveId & 0xFFu);
	data[CONFIG_UMB_OFFSET + UMB_SLAVE_ID_OFFSET + 1] = static_cast<uint8_t>((_slaveId & 0xFF00u) >> 8);

}

void EncodeVer0::setGsmApiBaseUrl(const std::string &password) {
	if (password.length() < GSM_API_BASE_URL_LEN) {
		const char * pStr = password.c_str();
		char * pDestination = reinterpret_cast<char*>(&data.at(0)) + GSM_API_BASE_URL_OFFSET + CONFIG_GSM_OFFSET;

		memset(pDestination, 0x00, GSM_API_BASE_URL_LEN);
		strncpy(pDestination, pStr, password.length());
	}
	else {
		throw std::runtime_error("Provided API base url is too long");
	}
}

void EncodeVer0::setLongitude(float _longitude) {
	void * ptr = &_longitude;

	uint32_t * uptr = (uint32_t*)ptr;

	uint32_t intermediate = *uptr;
	// VS2005 is complaining about that
	//uint32_t intermediate = reinterpret_cast<uint32_t>(*reinterpret_cast<uint32_t*>(ptr));

	data[CONFIG_BASIC_OFFSET + BASIC_LONGITUDE_OFFS] 		= static_cast<uint8_t>(intermediate 	& 0x000000FFu);
	data[CONFIG_BASIC_OFFSET + BASIC_LONGITUDE_OFFS + 1] 	= static_cast<uint8_t>((intermediate 	& 0x0000FF00u) >> 8);
	data[CONFIG_BASIC_OFFSET + BASIC_LONGITUDE_OFFS + 2] 	= static_cast<uint8_t>((intermediate 	& 0x00FF0000u) >> 16);
	data[CONFIG_BASIC_OFFSET + BASIC_LONGITUDE_OFFS + 3] 	= static_cast<uint8_t>((intermediate 	& 0xFF000000u) >> 24);

}

void EncodeVer0::setWxTransmitPeriod(uint8_t wxTransmitPeriod) {
	data[CONFIG_BASIC_OFFSET + BASIC_WX_TRANSMIT_PERIOD] = wxTransmitPeriod;

}

void EncodeVer0::setSymbol(AprsSymbol _symbol) {

	uint8_t byte = 0;

	switch (_symbol) {
	case SYMBOL_WIDE1_DIGI: byte = 1; break;
	case SYMBOL_HOUSE: byte = 2; break;
	case SYMBOL_RXIGATE: byte = 3; break;
	case SYMBOL_IGATE: byte = 4; break;
	case SYMBOL_DIGI: byte = 0; break;
	case SYMBOL_SAILBOAT: byte = 5; break;
	}

	data[CONFIG_BASIC_OFFSET + BASIC_SYMBOL_OFFSET] = byte;

}

void EncodeVer0::setRtuConfiguredSourceWinddirection(
		uint8_t __rtuSrcWinddirection) {
	data[CONFIG_RTU_OFFSET + RTU_WIND_DIR_OFFSET] = __rtuSrcWinddirection;

}

void EncodeVer0::setRtuConfiguredSourceWindspeed(uint8_t _rtuSrcWindspeed) {
	data[CONFIG_RTU_OFFSET + RTU_WIND_SPEED_OFFSET] = _rtuSrcWindspeed;

}

void EncodeVer0::setButtonTwoFunction(ButtonFunction _buttonTwo) {
	throw NotSupportedEx();	// TODO: not yet available

}

void EncodeVer0::setBeaconTransmitPeriod(uint8_t beaconTransmitPeriod) {
	data[CONFIG_BASIC_OFFSET + BASIC_BEACON_TRANSMIT_PERIOD] = beaconTransmitPeriod;
}

void EncodeVer0::setHumiditySrc(WeatherSource _humiditySource) {

	uint8_t byte = 0;

	switch(_humiditySource) {
		case WX_SOURCE_UMB:
			byte = 2;
			break;
		case WX_SOURCE_RTU:
			byte = 3;
			break;
		case WX_SOURCE_FULL_RTU:
			byte = 4;
			break;
		case WX_SOURCE_DAVIS_SERIAL:
			byte = 5;
			break;
		case WX_SOURCE_INTERNAL:
			byte = 1;
			break;
		case WX_SOURCE_INTERNAL_PT100:
			throw NotSupportedEx();	// TODO: not yet available
			break;
	}

	data[CONFIG_SOURCES_OFFSET + SOURCE_HUMIDITY_OFFSET] = byte;
}

void EncodeVer0::setSsid(uint8_t _ssid) {
	data[CONFIG_BASIC_OFFSET + BASIC_SSID_OFFSET] = _ssid;
}

void EncodeVer0::setDigiDelayInMsec(uint8_t _delayInMsec) {
	data[CONFIG_MODE_OFSET + MODE_DIGI_DELAY_100MSEC_OFFSET] = _delayInMsec;
}

bool EncodeVer0::decodeToFile(std::string _fn) {
	return false;
}

void EncodeVer0::setUmbChannelQnh(uint16_t _channelQnhPressure) {
	data[CONFIG_UMB_OFFSET + UMB_CHANNEL_QNH] = static_cast<uint8_t>(_channelQnhPressure & 0xFFu);
	data[CONFIG_UMB_OFFSET + UMB_CHANNEL_QNH + 1] = static_cast<uint8_t>((_channelQnhPressure & 0xFF00u) >> 8);

}

void EncodeVer0::setGsmAprsisServer(const std::string & server ) {
	if (server.length() < GSM_API_BASE_URL_LEN) {
		const char * pStr = server.c_str();
		char * pDestination = reinterpret_cast<char*>(&data.at(0)) + GSM_APRSIS_SERVER_OFFSET + CONFIG_GSM_OFFSET;

		memset(pDestination, 0x00, GSM_APRSIS_SERVER_LN);
		strncpy(pDestination, pStr, server.length());
	}
	else {
		throw std::runtime_error("Provided API base url is too long");
	}
}

void EncodeVer0::setGsmAprsisServerPort (uint16_t port) {
	data[GSM_APRSIS_PORT_OFFSET + CONFIG_GSM_OFFSET] 		= static_cast<uint8_t>(port 	& 0x00FFu);
	data[GSM_APRSIS_PORT_OFFSET + CONFIG_GSM_OFFSET + 1] 	= static_cast<uint8_t>((port 	& 0xFF00u) >> 8);

}

void EncodeVer0::setGsmAprsisPasscode	(uint32_t passcode) {
	data[CONFIG_GSM_OFFSET + GSM_APRSIS_PASSCODE_OFFSET] 		= static_cast<uint8_t>(passcode 	& 0x000000FFu);
	data[CONFIG_GSM_OFFSET + GSM_APRSIS_PASSCODE_OFFSET + 1] 	= static_cast<uint8_t>((passcode 	& 0x0000FF00u) >> 8);
	data[CONFIG_GSM_OFFSET + GSM_APRSIS_PASSCODE_OFFSET + 2] 	= static_cast<uint8_t>((passcode 	& 0x00FF0000u) >> 16);
	data[CONFIG_GSM_OFFSET + GSM_APRSIS_PASSCODE_OFFSET + 3] 	= static_cast<uint8_t>((passcode 	& 0xFF000000u) >> 24);

}
