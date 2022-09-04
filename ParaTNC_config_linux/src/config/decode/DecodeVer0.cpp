/*
 * DecodeVer0.cpp
 *
 *  Created on: Sep 3, 2022
 *      Author: mateusz
 */

#include "DecodeVer0.h"

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
#define MODE_VICTRON_OFFSET				0x7
#define MODE_DIGI_VISCOUS_OFFSET		0x8
#define MODE_DIGI_ONLY_SSID_OFFSET		0x9
#define MODE_DIGI_VISCOUS_DELAY_OFFSET	0xA
#define MODE_DIGI_DELAY_100MSEC_OFFSET	0xB
#define MODE_POWERSAVE_OFFSET			0xC
#define MODE_POWERSAVE_KEEP_GSM_OFFSET	0xD
#define MODE_GSM_OFFSET					0xE

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


DecodeVer0::~DecodeVer0() {
	// TODO Auto-generated destructor stub
}

bool DecodeVer0::getMs5611orBmeSensor() {
}

DigiFilter DecodeVer0::getDigiFilter() {
}

uint8_t DecodeVer0::getWxTransmitPeriod() {
}

ButtonFunction DecodeVer0::getButtonTwoFunction() {
}

bool DecodeVer0::getKeepGsmAlwaysOn() {
}

bool DecodeVer0::getGsmApiEnable() {
}

uint8_t DecodeVer0::getAnemometerPulsesConstant() {
}

uint8_t DecodeVer0::getRtuSlaveParity() {
}

uint8_t DecodeVer0::getCallsign() {
}

void DecodeVer0::getGsmApiBaseUrl(std::string &password) {
}

Digi DecodeVer0::getDigiEnabled() {
}

Powersave DecodeVer0::getPowersave() {
}

AprsPath DecodeVer0::getPath() {
}

uint8_t DecodeVer0::getRtuChannelWindgusts() {
}

bool DecodeVer0::getGsmAprsisEnable() {
}

bool DecodeVer0::getNorS() {
}

std::string DecodeVer0::getDigiFilterPrefixList(uint8_t entry) {
}

uint16_t DecodeVer0::getUmbChannelWinddirection() {
}

uint8_t DecodeVer0::getRtuChannelWinddirection() {
}

uint8_t DecodeVer0::getBeaconTransmitPeriod() {
}

uint8_t DecodeVer0::getDigiDelayInMsec() {
}

uint32_t DecodeVer0::getGsmPin() {
}

uint16_t DecodeVer0::getUmbChannelWindspeed() {
}

uint16_t DecodeVer0::getUmbChannelQnh() {
}

uint16_t DecodeVer0::getRtuSlaveSpeed() {
}

void DecodeVer0::getGsmApnName(std::string &apn) {
}

void DecodeVer0::getCallsign(std::string &call) {
}

uint16_t DecodeVer0::getUmbSlaveId() {
}

uint8_t DecodeVer0::getDigiRadiusFilter() {
}

bool DecodeVer0::getEorW() {
}

WeatherSource DecodeVer0::getWindSrc() {
}

AprsSymbol DecodeVer0::getSymbol() {
}

bool DecodeVer0::getVictronEnabled() {
}

bool DecodeVer0::getGsmEnabled() {
}

uint8_t DecodeVer0::getVisvousDelayInSec() {
}

bool DecodeVer0::getBeaconAtStartup() {
}

Rtu DecodeVer0::getRtuChannelConfig(uint8_t channel) {
}

uint8_t DecodeVer0::getRtuChannelTemperature() {
}

void DecodeVer0::getGsmApnUsername(std::string &username) {
}

ButtonFunction DecodeVer0::getButtonOneFunction() {
}

uint16_t DecodeVer0::getUmbSlaveClass() {
}

void DecodeVer0::getDescritpion(std::string &description) {
}

uint16_t DecodeVer0::getUmbChannelTemperature() {
}

uint16_t DecodeVer0::getUmbChannelWinggusts() {
}

WeatherSource DecodeVer0::getPressureSrc() {
}

bool DecodeVer0::getWxModbusEnabled() {
}

float DecodeVer0::getLongitude() {
}

float DecodeVer0::getLatitude() {
}

WeatherSource DecodeVer0::getTemperatureSrc() {
}

bool DecodeVer0::getRtuFullWindData() {
}

uint8_t DecodeVer0::getRtuSlaveStopBits() {
}

uint8_t DecodeVer0::getRtuChannelWindspeed() {
}

bool DecodeVer0::getDigiAlwaysMessageAndStatus() {
}

bool DecodeVer0::getWxUmbEnabled() {
}

bool DecodeVer0::getWxEnabled() {
}

bool DecodeVer0::getWxDavisEnabled() {
}

uint8_t DecodeVer0::getRtuChannelQnh() {
}

bool DecodeVer0::getWxDoubleTransmit() {
}

WeatherSource DecodeVer0::getHumiditySrc() {
}

void DecodeVer0::getGsmApnPassword(std::string &password) {
}
