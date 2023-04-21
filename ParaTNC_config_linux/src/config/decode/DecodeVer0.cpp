/*
 * DecodeVer0.cpp
 *
 *  Created on: Sep 3, 2022
 *      Author: mateusz
 */

#include "DecodeVer0.h"
#include "../../exceptions/NotSupportedEx.h"

#include <algorithm>

#define CONFIG_MODE_PGM_CNTR	0x0
#define CONFIG_MODE_OFSET		0x20			//	Current size: 0x10, free: 0x10
#define CONFIG_BASIC_OFFSET		0x40			//	Current size: 0x9C, free: 0x44
#define CONFIG_SOURCES_OFFSET	0x120			//	Current size: 0x4,  free: 0x1C
#define CONFIG_UMB_OFFSET		0x140			//	Current size: 0x10, free: 0x10
#define CONFIG_RTU_OFFSET		0x160			//	Current size: 0x54, free: 0x4C
#define CONFIG_GSM_OFFSET		0x200			//	Current size: 0xF8,
#define CONFIG__END__OFFSET		0x300

#define BASIC_CALLSIGN_OFFSET			0x0
#define BASIC_SSID_OFFSET				0x7
#define BASIC_LATITUDE_OFFSET			0x8
#define BASIC_NS_OFFSET					0xC
#define BASIC_LONGITUDE_OFFS			0x10
#define BASIC_WE_OFFSET					0x14
#define BASIC_COMMENT_OFFSET			0x15		// ln 128
	#define BASIC_COMMENT_LENGHT		128
#define BASIC_SYMBOL_OFFSET				0x95
#define BASIC_PATHTYPE_OFFSET			0x96
#define BASIC_BEACON_BOOTUP_OFFSET		0x97
#define BASIC_WX_TRANSMIT_PERIOD		0x98
#define BASIC_BEACON_TRANSMIT_PERIOD	0x99
#define BASIC_WX_DOUBLE_TRANSMIT		0x9A

#define MODE_DIGI_OFFSET				0x0
#define MODE_WX_OFFSET					0x1
#define MODE_WX_UMB_OFFSET				0x2
#define MODE_WX_MODBUS_OFFSET			0x3
#define MODE_WX_DAVIS_OFFSET			0x4
#define MODE_WX_MS5611_OR_BME_OFFSET	0x5
#define MODE_WX_ANEMOMETER_CONST_OFFSET	0x6
#define MODE_WX_DUST_SENSOR_OFFSET		0x7
#define MODE_WX_PT_SENSOR_OFFSET		0x8
#define MODE_VICTRON_OFFSET				0x9
#define MODE_DIGI_VISCOUS_OFFSET		0xA
#define MODE_DIGI_ONLY_SSID_OFFSET		0xB
#define MODE_DIGI_VISCOUS_DELAY_OFFSET	0xC
#define MODE_DIGI_DELAY_100MSEC_OFFSET	0xD
#define MODE_POWERSAVE_OFFSET			0xE
#define MODE_POWERSAVE_KEEP_GSM_OFFSET	0xF
#define MODE_GSM_OFFSET					0x10

#define SOURCE_TEMPERATURE_OFFSET		0x0
#define SOURCE_PRESSURE_OFFSET			0x1
#define SOURCE_HUMIDITY_OFFSET			0x2
#define SOURCE_WIND_OFFSET				0x3

#define UMB_SLAVE_CLASS_OFFSET			0x0
#define UMB_SLAVE_ID_OFFSET				0x2
#define UMB_CHANNEL_WINDSPEED			0x4
#define UMB_CHANNEL_WINDGUST			0x6
#define UMB_CHANNEL_WINDDIRECTION		0x8
#define UMB_CHANNEL_TEMPERATURE			0xA
#define UMB_CHANNEL_QNH					0xC

#define RTU_SLAVE_SPEED_OFFSET			0x0
#define RTU_SLAVE_PARITY_SPEED			0x2
#define RTU_SLAVE_STOP_BITS_OFFSET		0x3
#define RTU_USE_FULL_WIND_DATA_OFFSET	0x4
#define RTU_TEMPERATURE_SRC_OFFSET		0x5
#define RTU_HUMIDITY_SRC_OFFSET			0x6
#define RTU_PRESSURE_SRC_OFFSET			0x7
#define RTU_WIND_DIR_OFFSET				0x8
#define RTU_WIND_SPEED_OFFSET			0x9
#define RTU_WIND_GUSTS_OFFSET			0xA

#define RTU_SLAVE_CONFIG_BLOCK_OFFSET	0xE
#define RTU_SLAVE_CONFIG_BLOCK_SIZE		0xC

#define RTU_X_BUS_ADDRESS			0x0
#define RTU_X_FUNCTION				0x1
#define RTU_X_REGUSTER_ADDR			0x3
#define RTU_X_LENGHT				0x5
#define RTU_X_SCALLING_A			0x7
#define RTU_X_SCALLING_B			0x8
#define RTU_X_SCALLING_C			0x9
#define RTU_X_SCALLING_D			0xA
#define RTU_X_UNSIGNED_SIGNED		0xB

#define GSM_PIN_OFFSET				0x0
		#define GSM_PIN_LENGHT		5
#define GSM_APN_OFFSET				0x5
		#define GSM_APN_LENGHT		24
#define GSM_USERNAME_OFFSET			0x1D
		#define GSM_USERNAME_LEN	24
#define GSM_PASSWORD_OFFSET			0x35
		#define GSM_PASSWORD_LEN	24
#define GSM_API_ENABLE_OFFSET		0x4D
#define GSM_API_BASE_URL_OFFSET		0x4E
	#define GSM_API_BASE_URL_LEN	64
#define GSM_API_STATION_NAME_OFFSET	0x8E
	#define GSM_API_STATION_NAME_LN	32
#define GSM_APRSIS_ENABLE			0xAE
#define GSM_APRSIS_SERVER_OFFSET	0xAF
	#define GSM_APRSIS_SERVER_LN	64
#define GSM_APRSIS_PORT_OFFSET		0xF0
	#define GSM_APRSIS_PORT_LN		2
#define GSM_APRSIS_PASSCODE_OFFSET	0xF4
#define GSM_SMS_WX_OFFSET

DecodeVer0::DecodeVer0(const std::vector<uint8_t> &configData) : data(configData) {
}

DecodeVer0::~DecodeVer0() {
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

uint8_t DecodeVer0::getRtuChannelWindgusts() {
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

bool DecodeVer0::getNorS() {
	uint8_t byte = data.at(CONFIG_BASIC_OFFSET + BASIC_NS_OFFSET );

	if (byte == 1) {
		return true;
	}
	else {
		return false;
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

uint8_t DecodeVer0::getRtuChannelWinddirection() {
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
	uint8_t lsb_byte = data.at(CONFIG_MODE_OFSET + MODE_DIGI_DELAY_100MSEC_OFFSET);
	uint8_t nd_byte = data.at(CONFIG_MODE_OFSET + MODE_DIGI_DELAY_100MSEC_OFFSET + 1);
	uint8_t rd_byte = data.at(CONFIG_MODE_OFSET + MODE_DIGI_DELAY_100MSEC_OFFSET + 2);
	uint8_t msb_byte = data.at(CONFIG_MODE_OFSET + MODE_DIGI_DELAY_100MSEC_OFFSET + 3);


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
	} while(startIt != endIt);

	//std::for_each(startIt, endIt, [&apn](uint8_t b) { apn.append(1, (char) b);});
}

void DecodeVer0::getCallsign(std::string &call) {
	std::size_t offset = BASIC_CALLSIGN_OFFSET + CONFIG_BASIC_OFFSET;

	std::vector<uint8_t>::const_iterator startIt = data.begin() + offset;
	std::vector<uint8_t>::const_iterator endIt = data.begin() + offset + GSM_APN_LENGHT;

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

bool DecodeVer0::getEorW() {
	uint8_t byte = data.at(CONFIG_BASIC_OFFSET + BASIC_WE_OFFSET);

	if (byte == 1) {
		return true;
	}
	else {
		return false;
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

Rtu DecodeVer0::getRtuChannelConfig(uint8_t channel) {
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

uint8_t DecodeVer0::getRtuChannelTemperature() {
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

	//std::for_each(startIt, endIt, [&username](uint8_t b) { username.append(1, (char) b);});
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
	} while(startIt != endIt);

	//std::for_each(startIt, endIt, [&description](uint8_t b) { description.append(1, (char) b);});
}

uint16_t DecodeVer0::getUmbChannelTemperature() {
	uint8_t lsb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_TEMPERATURE );
	uint8_t msb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_TEMPERATURE + 1);

	return lsb_byte | (msb_byte << 8);
}

uint16_t DecodeVer0::getUmbChannelWinggusts() {
	uint8_t lsb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_TEMPERATURE );
	uint8_t msb_byte = data.at(CONFIG_UMB_OFFSET + UMB_CHANNEL_TEMPERATURE + 1);

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


	return (float)(lsb_byte | (nd_byte << 8) | (rd_byte << 16) | (msb_byte << 24));
}

float DecodeVer0::getLatitude() {
	uint8_t lsb_byte = data.at(CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET);
	uint8_t nd_byte = data.at(CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET + 1);
	uint8_t rd_byte = data.at(CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET + 2);
	uint8_t msb_byte = data.at(CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET + 3);


	return (float)(lsb_byte | (nd_byte << 8) | (rd_byte << 16) | (msb_byte << 24));
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

uint8_t DecodeVer0::getRtuChannelWindspeed() {
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

uint8_t DecodeVer0::getRtuChannelQnh() {
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
	} while(startIt != endIt);

	//std::for_each(startIt, endIt, [&password](uint8_t b) { password.append(1, (char) b);});
}
