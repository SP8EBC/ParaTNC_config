/*
 * DecodeVer0.cpp
 *
 *  Created on: Sep 3, 2022
 *      Author: mateusz
 */

#include "stdafx.h"

#include "DecodeVer0.h"
#include "../Ver0Map.h"
#include "../../exceptions/NotSupportedEx.h"

#include <algorithm>
#include <fstream>

#include <time.h>



DecodeVer0::DecodeVer0(const std::vector<uint8_t> &configData) : data(configData) {
}

DecodeVer0::~DecodeVer0() {

}

bool DecodeVer0::decodeToFile(std::string _fn) {

	bool out = false;

	std::fstream outputTextFile;

	char datetime_buffer[80];

	// get current time
    time_t t = time(0);   // get time now
#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	tm* now = localtime(&t);
#else
	tm* now = localtime(&t);
#endif


    strftime(datetime_buffer, sizeof(datetime_buffer), "%Y-%m-%d %HH:%MM", now);

	// open file and wipe it's content
	outputTextFile.open(_fn.c_str(), std::ios::out);

	// check if file has been opened
	if (outputTextFile.is_open()) {
		outputTextFile << "--------------------------------------------------------" << std::endl;
		outputTextFile << "--- Configuration dump generated at: " << datetime_buffer << " --- " << std::endl;
		outputTextFile << "--------------------------------------------------------" << std::endl;
	}

	return out;
}

uint32_t DecodeVer0::getProgrammingCounter() {
	uint8_t lsb_byte = data.at(CONFIG_MODE_PGM_CNTR);
	uint8_t nd_byte = data.at(CONFIG_MODE_PGM_CNTR + 1);
	uint8_t rd_byte = data.at(CONFIG_MODE_PGM_CNTR + 2);
	uint8_t msb_byte = data.at(CONFIG_MODE_PGM_CNTR + 3);


	return lsb_byte | (nd_byte << 8) | (rd_byte << 16) | (msb_byte << 24);
}

bool DecodeVer0::getMs5611orBmeSensor() {
	bool output = false;

	uint8_t byte = data.at(CONFIG_MODE_OFSET + MODE_WX_MS5611_OR_BME_OFFSET );

	output = (byte != 0) ? true : false;

	return output;
}

DigiFilter DecodeVer0::getDigiFilter() {

	DigiFilter out;

	uint8_t only_ssid = data.at(CONFIG_MODE_OFSET + MODE_DIGI_ONLY_SSID_OFFSET);

	if (only_ssid == 1) {
		out = DIGI_FILTER_SSID789_ONLY;
	}
	else {
		out = DIGI_FILTER_OFF;
	}

	return out;
}

uint8_t DecodeVer0::getWxTransmitPeriod() {
	return data.at(CONFIG_BASIC_OFFSET + BASIC_WX_TRANSMIT_PERIOD);
}

ButtonFunction DecodeVer0::getButtonTwoFunction() {
	throw NotSupportedEx(); // TODO: not yet available
}

bool DecodeVer0::getKeepGsmAlwaysOn() {
	bool output = false;

	uint8_t byte = data.at(CONFIG_MODE_OFSET + MODE_POWERSAVE_KEEP_GSM_OFFSET );

	output = (byte == 1) ? true : false;

	return output;
}

bool DecodeVer0::getGsmApiEnable() {
	bool output = false;

	uint8_t byte = data.at(CONFIG_GSM_OFFSET + GSM_API_ENABLE_OFFSET );

	output = (byte == 1) ? true : false;

	return output;
}

uint8_t DecodeVer0::getAnemometerPulsesConstant() {
	return data.at(CONFIG_MODE_OFSET + MODE_WX_ANEMOMETER_CONST_OFFSET);

}

uint8_t DecodeVer0::getRtuSlaveParity() {
	return data.at(CONFIG_MODE_OFSET + MODE_WX_ANEMOMETER_CONST_OFFSET);

}

uint8_t DecodeVer0::getSsid() {
	return data.at(CONFIG_BASIC_OFFSET + BASIC_SSID_OFFSET);
}

void DecodeVer0::getGsmApiBaseUrl(std::string &password) {
	std::size_t offset = GSM_API_BASE_URL_OFFSET + CONFIG_GSM_OFFSET;

	std::vector<uint8_t>::const_iterator startIt = data.begin() + offset;
	std::vector<uint8_t>::const_iterator endIt = data.begin() + offset + GSM_API_BASE_URL_LEN;

	password.clear();

	do {
		password.append(1, (char) *startIt);

		startIt++;

		if (*startIt == 0x00) {
			break;
		}
	} while(startIt != endIt);

	//std::for_each(startIt, endIt, [&password](uint8_t b) { password.append(1, (char) b);});


}

Digi DecodeVer0::getDigiEnabled() {
	Digi out;

	uint8_t digi_viscous = data.at(CONFIG_MODE_OFSET + MODE_DIGI_VISCOUS_OFFSET);

	uint8_t digi = data.at(CONFIG_MODE_OFSET + MODE_DIGI_OFFSET);

	if (digi == 1 && digi_viscous == 0) {
		out = DIGI_WIDE1;
	}
	else if (digi == 1 && digi_viscous == 1) {
		out = DIGI_VISCOUS_WIDE1;
	}
	else {
		out = DIGI_OFF;
	}

	return out;
}

Powersave DecodeVer0::getPowersave() {
	Powersave out;

	uint8_t pwsave = data.at(CONFIG_MODE_OFSET + MODE_POWERSAVE_OFFSET);

	if (pwsave == 1) {
		out = PWSAVE_NORMAL;
	}
	else if (pwsave == 2) {
		out = PWSAVE_AGGRESV;
	}
	else {
		out = PWSAVE_NONE;
	}

	return out;
}

AprsPath DecodeVer0::getPath() {
	AprsPath out;

	uint8_t path = data.at(CONFIG_BASIC_OFFSET + BASIC_PATHTYPE_OFFSET);

	if (path == 1) {
		out = PATH_WIDE11;
	}
	else if (path == 2) {
		out = PATH_WIDE21;
	}
	else {
		out = PATH_NOPATH;
	}

	return out;
}

uint8_t DecodeVer0::getRtuConfiguredSourceWindgusts() {
	uint8_t byte = data.at(CONFIG_RTU_OFFSET + RTU_WIND_GUSTS_OFFSET );

	return byte;
}

bool DecodeVer0::getGsmAprsisEnable() {
	uint8_t byte = data.at(CONFIG_GSM_OFFSET + GSM_APRSIS_ENABLE );

	if (byte == 1) {
		return true;
	}
	else {
		return false;
	}
}

CardinalDirection DecodeVer0::getNorS() {
	uint8_t byte = data.at(CONFIG_BASIC_OFFSET + BASIC_NS_OFFSET );

	if (byte == 'S') {
		return SOUTH;
	}
	else {
		return NORTH;
	}
}

std::string DecodeVer0::getDigiFilterPrefixList(uint8_t entry) {
	throw NotSupportedEx();	// TODO: not yet available
}

uint16_t DecodeVer0::getUmbChannelWinddirection() {
	uint8_t lsb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDDIRECTION );
	uint8_t msb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDDIRECTION + 1);

	return lsb_byte | (msb_byte << 8);
}

uint8_t DecodeVer0::getRtuConfiguredSourceWinddirection() {
	uint8_t byte = data.at(CONFIG_RTU_OFFSET + RTU_WIND_DIR_OFFSET );

	return byte;
}

uint8_t DecodeVer0::getBeaconTransmitPeriod() {
	return data.at(CONFIG_BASIC_OFFSET + BASIC_BEACON_TRANSMIT_PERIOD);

}

uint8_t DecodeVer0::getDigiDelayInMsec() {
	return data.at(CONFIG_MODE_OFSET + MODE_DIGI_DELAY_100MSEC_OFFSET);

}

uint32_t DecodeVer0::getGsmPin() {
	uint8_t lsb_byte = data.at(CONFIG_GSM_OFFSET + GSM_PIN_OFFSET);
	uint8_t nd_byte = data.at(CONFIG_GSM_OFFSET + GSM_PIN_OFFSET + 1);
	uint8_t rd_byte = data.at(CONFIG_GSM_OFFSET + GSM_PIN_OFFSET + 2);
	uint8_t msb_byte = data.at(CONFIG_GSM_OFFSET + GSM_PIN_OFFSET + 3);


	return lsb_byte | (nd_byte << 8) | (rd_byte << 16) | (msb_byte << 24);

}

uint16_t DecodeVer0::getUmbChannelWindspeed() {
	uint8_t lsb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDSPEED );
	uint8_t msb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDSPEED + 1);

	return lsb_byte | (msb_byte << 8);
}

uint16_t DecodeVer0::getUmbChannelQnh() {
	uint8_t lsb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_QNH );
	uint8_t msb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_QNH + 1);

	return lsb_byte | (msb_byte << 8);
}

uint16_t DecodeVer0::getRtuSlaveSpeed() {
	uint8_t lsb_byte = data.at(CONFIG_RTU_OFFSET + RTU_SLAVE_SPEED_OFFSET );
	uint8_t msb_byte = data.at(CONFIG_RTU_OFFSET + RTU_SLAVE_SPEED_OFFSET + 1);

	return lsb_byte | (msb_byte << 8);
}

void DecodeVer0::getGsmApnName(std::string &apn) {
	std::size_t offset = GSM_APN_OFFSET + CONFIG_GSM_OFFSET;

	std::vector<uint8_t>::const_iterator startIt = data.begin() + offset;
	std::vector<uint8_t>::const_iterator endIt = data.begin() + offset + GSM_APN_LENGHT;

	apn.clear();

	do {
		apn.append(1, (char) *startIt);

		startIt++;

		if (*startIt == 0x00) {
			break;
		}
	} while(startIt != endIt);

	//std::for_each(startIt, endIt, [&apn](uint8_t b) { apn.append(1, (char) b);});
}

void DecodeVer0::getCallsign(std::string &call) {
	std::size_t offset = BASIC_CALLSIGN_OFFSET + CONFIG_BASIC_OFFSET;

	std::vector<uint8_t>::const_iterator startIt = data.begin() + offset;
	std::vector<uint8_t>::const_iterator endIt = data.begin() + offset + 6;

	call.clear();

	do {
		call.append(1, (char) *startIt);

		startIt++;
	} while(startIt != endIt);

	//std::for_each(startIt, endIt, [&call](uint8_t b) { call.append(1, (char) b);});
}

uint16_t DecodeVer0::getUmbSlaveId() {
	uint8_t lsb_byte = data.at(CONFIG_UMB_OFFSET + UMB_SLAVE_ID_OFFSET );
	uint8_t msb_byte = data.at(CONFIG_UMB_OFFSET + UMB_SLAVE_ID_OFFSET + 1);

	return lsb_byte | (msb_byte << 8);
}

uint8_t DecodeVer0::getDigiRadiusFilter() {
	throw NotSupportedEx(); // TODO: not yet available

}

CardinalDirection DecodeVer0::getEorW() {
	uint8_t byte = data.at(CONFIG_BASIC_OFFSET + BASIC_WE_OFFSET);

	if (byte == 'E') {
		return EAST;
	}
	else {
		return WEST;
	}
}

WeatherSource DecodeVer0::getWindSrc() {
	WeatherSource out;

	uint8_t byte = data.at(CONFIG_SOURCES_OFFSET + SOURCE_WIND_OFFSET);

	if (byte == 2) {
		out = WX_SOURCE_UMB;
	}
	else if (byte == 3) {
		out = WX_SOURCE_RTU;
	}
	else if (byte == 4) {
		out = WX_SOURCE_FULL_RTU;
	}
	else if (byte == 5) {
		out = WX_SOURCE_DAVIS_SERIAL;
	}
	else if (byte == 6) {
		out = WX_SOURCE_INTERNAL_PT100;
	}
	else {
		out = WX_SOURCE_INTERNAL;
	}

	return out;
}

AprsSymbol DecodeVer0::getSymbol() {

	AprsSymbol out;

	uint8_t byte = data.at(CONFIG_BASIC_OFFSET + BASIC_SYMBOL_OFFSET);

	if (byte == 1) {
		out = SYMBOL_WIDE1_DIGI;
	}
	else if (byte == 2) {
		out = SYMBOL_HOUSE;
	}
	else if (byte == 3) {
		out = SYMBOL_RXIGATE;
	}
	else if (byte == 4) {
		out = SYMBOL_IGATE;
	}
	else if (byte == 5) {
		out = SYMBOL_SAILBOAT;
	}
	else {
		out = SYMBOL_DIGI;
	}

	return out;

}

bool DecodeVer0::getVictronEnabled() {
	uint8_t byte = data.at(CONFIG_MODE_OFSET + MODE_VICTRON_OFFSET);

	if (byte == 1) {
		return true;
	}
	else {
		return false;
	}
}

bool DecodeVer0::getGsmEnabled() {
	uint8_t byte = data.at(CONFIG_MODE_OFSET + MODE_GSM_OFFSET);

	if (byte == 1) {
		return true;
	}
	else {
		return false;
	}
}

uint8_t DecodeVer0::getVisvousDelayInSec() {
	return data.at(CONFIG_MODE_OFSET + MODE_DIGI_VISCOUS_DELAY_OFFSET);

}

bool DecodeVer0::getBeaconAtStartup() {
	uint8_t byte = data.at(CONFIG_BASIC_OFFSET + BASIC_BEACON_BOOTUP_OFFSET);

	if (byte == 1) {
		return true;
	}
	else {
		return false;
	}
}

Rtu DecodeVer0::getRtuSourceConfig(uint8_t channel) {
	/**
	 * 	uint8_t busAddress;
	uint8_t function;
	uint16_t registerAddress;
	uint16_t lenght;
	uint8_t coeff_a;
	uint8_t coeff_b;
	uint8_t coeff_c;
	uint8_t coeff_d;
	bool unsignedSigned;
	 */

	Rtu out;

	std::size_t channelOffset = CONFIG_RTU_OFFSET + RTU_SLAVE_CONFIG_BLOCK_OFFSET + (RTU_SLAVE_CONFIG_BLOCK_SIZE * channel);

	out.function = data.at(RTU_X_FUNCTION + channelOffset);
	out.coeff_a = data.at(RTU_X_SCALLING_A + channelOffset);
	out.coeff_b = data.at(RTU_X_SCALLING_B + channelOffset);
	out.coeff_c = data.at(RTU_X_SCALLING_C + channelOffset);
	out.coeff_d = data.at(RTU_X_SCALLING_D + channelOffset);

	out.lenght = data.at(RTU_X_LENGHT + channelOffset) | (data.at(RTU_X_LENGHT + channelOffset + 1) << 8);
	out.registerAddress = data.at(RTU_X_REGUSTER_ADDR + channelOffset) | (data.at(RTU_X_REGUSTER_ADDR + channelOffset + 1) << 8);



	return out;
}

uint8_t DecodeVer0::getRtuConfiguredSourceTemperature() {
	uint8_t byte = data.at(CONFIG_RTU_OFFSET + RTU_TEMPERATURE_SRC_OFFSET );

	return byte;
}

void DecodeVer0::getGsmApnUsername(std::string &username) {
	std::size_t offset = GSM_USERNAME_OFFSET + CONFIG_GSM_OFFSET;

	std::vector<uint8_t>::const_iterator startIt = data.begin() + offset;
	std::vector<uint8_t>::const_iterator endIt = data.begin() + offset + GSM_USERNAME_LEN;

	if (*startIt == 0xFF || *startIt == 0x00) {
		return;
	}

	username.clear();

	while (startIt != endIt) {
		username.push_back(*startIt);
		startIt++;

		if (*startIt == 0x00) {
			break;
		}
	}
}

ButtonFunction DecodeVer0::getButtonOneFunction() {
	throw NotSupportedEx();	// TODO:

}

uint16_t DecodeVer0::getUmbSlaveClass() {
	uint8_t lsb_byte = data.at(CONFIG_UMB_OFFSET + UMB_SLAVE_CLASS_OFFSET );
	uint8_t msb_byte = data.at(CONFIG_UMB_OFFSET + UMB_SLAVE_CLASS_OFFSET + 1);

	return lsb_byte | (msb_byte << 8);
}

void DecodeVer0::getDescritpion(std::string &description) {
	std::size_t offset = BASIC_COMMENT_OFFSET + CONFIG_BASIC_OFFSET;

	std::vector<uint8_t>::const_iterator startIt = data.begin() + offset;
	std::vector<uint8_t>::const_iterator endIt = data.begin() + offset + BASIC_COMMENT_LENGHT;

	if (*startIt == 0xFF || *startIt == 0x00) {
		return;
	}

	description.clear();

	do {
		description.append(1, (char) *startIt);

		startIt++;

		if (*startIt == 0x00) {
			break;
		}
	} while(startIt != endIt);

}

uint16_t DecodeVer0::getUmbChannelTemperature() {
	uint8_t lsb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_TEMPERATURE );
	uint8_t msb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_TEMPERATURE + 1);

	return lsb_byte | (msb_byte << 8);
}

uint16_t DecodeVer0::getUmbChannelWinggusts() {
	uint8_t lsb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDGUST );
	uint8_t msb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDGUST + 1);

	return lsb_byte | (msb_byte << 8);
}

WeatherSource DecodeVer0::getPressureSrc() {
	WeatherSource out;

	uint8_t byte = data.at(CONFIG_SOURCES_OFFSET + SOURCE_PRESSURE_OFFSET);

	if (byte == 2) {
		out = WX_SOURCE_UMB;
	}
	else if (byte == 3) {
		out = WX_SOURCE_RTU;
	}
	else if (byte == 4) {
		out = WX_SOURCE_FULL_RTU;
	}
	else if (byte == 5) {
		out = WX_SOURCE_DAVIS_SERIAL;
	}
	else {
		out = WX_SOURCE_INTERNAL;
	}

	return out;
}

bool DecodeVer0::getWxModbusEnabled() {
	uint8_t byte = data.at(CONFIG_MODE_OFSET + MODE_WX_MODBUS_OFFSET);

	if (byte == 1) {
		return true;
	}
	else {
		return false;
	}
}

float DecodeVer0::getLongitude() {
	uint8_t lsb_byte = data.at(CONFIG_BASIC_OFFSET + BASIC_LONGITUDE_OFFS);
	uint8_t nd_byte = data.at(CONFIG_BASIC_OFFSET + BASIC_LONGITUDE_OFFS + 1);
	uint8_t rd_byte = data.at(CONFIG_BASIC_OFFSET + BASIC_LONGITUDE_OFFS + 2);
	uint8_t msb_byte = data.at(CONFIG_BASIC_OFFSET + BASIC_LONGITUDE_OFFS + 3);


	uint32_t out = (lsb_byte | (nd_byte << 8) | (rd_byte << 16) | (msb_byte << 24));

	float out_float = *(float*)&out;

	return out_float;
}

float DecodeVer0::getLatitude() {
	uint8_t lsb_byte = data.at(CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET);
	uint8_t nd_byte = data.at(CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET + 1);
	uint8_t rd_byte = data.at(CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET + 2);
	uint8_t msb_byte = data.at(CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET + 3);

	uint32_t out = (lsb_byte | (nd_byte << 8) | (rd_byte << 16) | (msb_byte << 24));

	float out_float = *(float*)&out;

	return out_float;
}

WeatherSource DecodeVer0::getTemperatureSrc() {
	WeatherSource out;

	uint8_t byte = data.at(CONFIG_SOURCES_OFFSET + SOURCE_TEMPERATURE_OFFSET);

	if (byte == 2) {
		out = WX_SOURCE_UMB;
	}
	else if (byte == 3) {
		out = WX_SOURCE_RTU;
	}
	else if (byte == 4) {
		out = WX_SOURCE_FULL_RTU;
	}
	else if (byte == 5) {
		out = WX_SOURCE_DAVIS_SERIAL;
	}
	else {
		out = WX_SOURCE_INTERNAL;
	}

	return out;
}

bool DecodeVer0::getRtuFullWindData() {
	uint8_t byte = data.at(CONFIG_RTU_OFFSET + RTU_USE_FULL_WIND_DATA_OFFSET);

	if (byte == 1) {
		return true;
	}
	else {
		return false;
	}
}

uint8_t DecodeVer0::getRtuSlaveStopBits() {
	uint8_t byte = data.at(CONFIG_RTU_OFFSET + RTU_SLAVE_STOP_BITS_OFFSET );

	return byte;
}

uint8_t DecodeVer0::getRtuConfiguredSourceWindspeed() {
	uint8_t byte = data.at(CONFIG_RTU_OFFSET + RTU_WIND_SPEED_OFFSET );

	return byte;
}

bool DecodeVer0::getDigiAlwaysMessageAndStatus() {
	throw NotSupportedEx();	// TODO:

}

bool DecodeVer0::getWxUmbEnabled() {
	uint8_t byte = data.at(CONFIG_MODE_OFSET + MODE_WX_UMB_OFFSET);

	if (byte == 1) {
		return true;
	}
	else {
		return false;
	}
}

bool DecodeVer0::getWxEnabled() {
	uint8_t byte = data.at(CONFIG_MODE_OFSET + MODE_WX_OFFSET);

	if (byte == 1) {
		return true;
	}
	else {
		return false;
	}
}

bool DecodeVer0::getWxDavisEnabled() {
	uint8_t byte = data.at(CONFIG_MODE_OFSET + MODE_WX_DAVIS_OFFSET);

	if (byte == 1) {
		return true;
	}
	else {
		return false;
	}
}

uint8_t DecodeVer0::getRtuConfiguredSourceQnh() {
	uint8_t byte = data.at(CONFIG_RTU_OFFSET + RTU_PRESSURE_SRC_OFFSET );

	return byte;
}

bool DecodeVer0::getWxDoubleTransmit() {
	uint8_t byte = data.at(CONFIG_BASIC_OFFSET + BASIC_WX_DOUBLE_TRANSMIT);

	if (byte == 1) {
		return true;
	}
	else {
		return false;
	}
}

WeatherSource DecodeVer0::getHumiditySrc() {
	WeatherSource out;

	uint8_t byte = data.at(CONFIG_SOURCES_OFFSET + SOURCE_HUMIDITY_OFFSET);

	if (byte == 2) {
		out = WX_SOURCE_UMB;
	}
	else if (byte == 3) {
		out = WX_SOURCE_RTU;
	}
	else if (byte == 4) {
		out = WX_SOURCE_FULL_RTU;
	}
	else if (byte == 5) {
		out = WX_SOURCE_DAVIS_SERIAL;
	}
	else {
		out = WX_SOURCE_INTERNAL;
	}

	return out;
}

void DecodeVer0::getGsmApnPassword(std::string &password) {
	std::size_t offset = GSM_PASSWORD_OFFSET + CONFIG_GSM_OFFSET;

	std::vector<uint8_t>::const_iterator startIt = data.begin() + offset;
	std::vector<uint8_t>::const_iterator endIt = data.begin() + offset + GSM_USERNAME_LEN;

	if (*startIt == 0xFF || *startIt == 0x00) {
		return;
	}

	password.clear();

	do {
		password.append(1, (char) *startIt);

		startIt++;

		if (*startIt == 0x00) {
			break;
		}
	} while(startIt != endIt);

}

void DecodeVer0::getGsmAprsisServer(std::string & server ) {
	std::size_t offset = GSM_APRSIS_SERVER_OFFSET + CONFIG_GSM_OFFSET;

	std::vector<uint8_t>::const_iterator startIt = data.begin() + offset;
	std::vector<uint8_t>::const_iterator endIt = data.begin() + offset + GSM_APRSIS_SERVER_LN;

	if (*startIt == 0xFF || *startIt == 0x00) {
		return;
	}

	server.clear();

	do {
		server.append(1, (char) *startIt);

		startIt++;

		if (*startIt == 0x00) {
			break;
		}
	} while(startIt != endIt);
}

uint16_t DecodeVer0::getGsmAprsisServerPort () {
	uint8_t lsb_byte = data.at(CONFIG_GSM_OFFSET + GSM_APRSIS_PORT_OFFSET );
	uint8_t msb_byte = data.at(CONFIG_GSM_OFFSET + GSM_APRSIS_PORT_OFFSET + 1);

	return lsb_byte | (msb_byte << 8);
}

uint32_t DecodeVer0::getGsmAprsisPasscode	() {
	uint8_t lsb_byte = data.at(CONFIG_GSM_OFFSET 	+ GSM_APRSIS_PASSCODE_OFFSET);
	uint8_t nd_byte = data.at(CONFIG_GSM_OFFSET 	+ GSM_APRSIS_PASSCODE_OFFSET + 1);
	uint8_t rd_byte = data.at(CONFIG_GSM_OFFSET 	+ GSM_APRSIS_PASSCODE_OFFSET + 2);
	uint8_t msb_byte = data.at(CONFIG_GSM_OFFSET 	+ GSM_APRSIS_PASSCODE_OFFSET + 3);


	return lsb_byte | (nd_byte << 8) | (rd_byte << 16) | (msb_byte << 24);
}
