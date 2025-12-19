/*
 * ConfigVer0.cpp
 *
 *  Created on: Nov 18, 2025
 *      Author: mateusz
 */

#include "ConfigVer0.h"
#include "ValidateVer0.h"
#include "Ver0Map.h"
#include <iostream>

// clang-format off

BasicConfig::BasicConfig (const std::vector<uint8_t> &data) : configData (data)
{
}

ModeConfig::ModeConfig (const std::vector<uint8_t> &data) : configData (data)
{
}

SourceConfig::SourceConfig (const std::vector<uint8_t> &data) : configData (data)
{
}

UmbConfig::UmbConfig (const std::vector<uint8_t> &data) : configData (data)
{
}

RtuConfig::RtuConfig (const std::vector<uint8_t> &data) : configData (data)
{
}

GsmConfig::GsmConfig (const std::vector<uint8_t> &data) : configData (data)
{
}

// ============================================================================
// BASIC Configuration Implementation
// ============================================================================

std::string BasicConfig::readString(size_t offset, size_t maxLength) const {
    if (offset + maxLength > configData.size()) {
        throw std::out_of_range("Config data access out of bounds");
    }
    std::string result;
    for (size_t i = 0; i < maxLength; ++i) {
        uint8_t byte = configData[offset + i];
        if (byte == 0) break; // Null terminator
        result += static_cast<char>(byte);
    }
    return result;
}

void BasicConfig::writeString(size_t offset, size_t maxLength, const std::string& value) {
    if (offset + maxLength > configData.size()) {
        throw std::out_of_range("Config data access out of bounds");
    }
    if (value.length() >= maxLength) {
        throw std::invalid_argument("String too long for field");
    }
    auto& mutableData = const_cast<std::vector<uint8_t>&>(configData);
    std::memcpy(&mutableData[offset], value.c_str(), value.length());
    mutableData[offset + value.length()] = 0; // Null terminator
    // Clear remaining bytes
    for (size_t i = value.length() + 1; i < maxLength; ++i) {
        mutableData[offset + i] = 0;
    }
}

void BasicConfig::getCallsign(std::string& call) const {
    call = readString(CONFIG_BASIC_OFFSET + BASIC_CALLSIGN_OFFSET, 7);
}

std::string BasicConfig::getCallsign() const {
    return readString(CONFIG_BASIC_OFFSET + BASIC_CALLSIGN_OFFSET, 7);
}

uint8_t BasicConfig::getSsid() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_SSID_OFFSET);
}

float BasicConfig::getLatitude() const {
    return readValue<float>(CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET);
}

uint8_t BasicConfig::getNs() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_NS_OFFSET);
}

float BasicConfig::getLongitude() const {
    return readValue<float>(CONFIG_BASIC_OFFSET + BASIC_LONGITUDE_OFFS);
}

uint8_t BasicConfig::getWe() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_WE_OFFSET);
}

void BasicConfig::getComment(std::string& comment) const {
    comment = readString(CONFIG_BASIC_OFFSET + BASIC_COMMENT_OFFSET, BASIC_COMMENT_LENGHT);
}

std::string BasicConfig::getComment() const {
    return readString(CONFIG_BASIC_OFFSET + BASIC_COMMENT_OFFSET, BASIC_COMMENT_LENGHT);
}

uint8_t BasicConfig::getSymbol() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_SYMBOL_OFFSET);
}

uint8_t BasicConfig::getPathType() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_PATHTYPE_OFFSET);
}

bool BasicConfig::getBeaconBootup() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_BEACON_BOOTUP_OFFSET);
}

uint8_t BasicConfig::getWxTransmitPeriod() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_WX_TRANSMIT_PERIOD);
}

uint8_t BasicConfig::getBeaconTransmitPeriod() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_BEACON_TRANSMIT_PERIOD);
}

bool BasicConfig::getWxDoubleTransmit() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_WX_DOUBLE_TRANSMIT);
}

void BasicConfig::setCallsign(const std::string& call) {
    writeString(CONFIG_BASIC_OFFSET + BASIC_CALLSIGN_OFFSET, 7, call);
}

void BasicConfig::setSsid(uint8_t ssid) {
    writeValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_SSID_OFFSET, ssid);
}

void BasicConfig::setLatitude(float latitude) {
    writeValue<float>(CONFIG_BASIC_OFFSET + BASIC_LATITUDE_OFFSET, latitude);
}

void BasicConfig::setNs(uint8_t ns) {
    writeValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_NS_OFFSET, ns);
}

void BasicConfig::setLongitude(float longitude) {
    writeValue<float>(CONFIG_BASIC_OFFSET + BASIC_LONGITUDE_OFFS, longitude);
}

void BasicConfig::setWe(uint8_t we) {
    writeValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_WE_OFFSET, we);
}

void BasicConfig::setComment(const std::string& comment) {
    writeString(CONFIG_BASIC_OFFSET + BASIC_COMMENT_OFFSET, BASIC_COMMENT_LENGHT, comment);
}

void BasicConfig::setSymbol(uint8_t symbol) {
    writeValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_SYMBOL_OFFSET, symbol);
}

void BasicConfig::setPathType(uint8_t pathType) {
    writeValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_PATHTYPE_OFFSET, pathType);
}

void BasicConfig::setBeaconBootup(bool bootup) {
    writeValue<bool>(CONFIG_BASIC_OFFSET + BASIC_BEACON_BOOTUP_OFFSET, bootup);
}

void BasicConfig::setWxTransmitPeriod(uint8_t period) {
    writeValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_WX_TRANSMIT_PERIOD, period);
}

void BasicConfig::setBeaconTransmitPeriod(uint8_t period) {
    writeValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_BEACON_TRANSMIT_PERIOD, period);
}

void BasicConfig::setWxDoubleTransmit(bool doubleTransmit) {
    writeValue<bool>(CONFIG_BASIC_OFFSET + BASIC_WX_DOUBLE_TRANSMIT, doubleTransmit);
}

// ============================================================================
// MODE Configuration Implementation
// ============================================================================

uint8_t ModeConfig::getDigi() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_OFFSET); }
uint8_t ModeConfig::getWx() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_OFFSET); }
bool ModeConfig::getWxUmb() const { return readValue<bool>(CONFIG_MODE_OFSET + MODE_WX_UMB_OFFSET); }
bool ModeConfig::getWxModbus() const { return readValue<bool>(CONFIG_MODE_OFSET + MODE_WX_MODBUS_OFFSET); }
bool ModeConfig::getWxDavis() const { return readValue<bool>(CONFIG_MODE_OFSET + MODE_WX_DAVIS_OFFSET); }
bool ModeConfig::getWxMs5611OrBme() const { return readValue<bool>(CONFIG_MODE_OFSET + MODE_WX_MS5611_OR_BME_OFFSET); }
uint8_t ModeConfig::getWxAnemometerConst() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_ANEMOMETER_CONST_OFFSET); }
uint8_t ModeConfig::getWxDustSensor() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_DUST_SENSOR_OFFSET); }
uint8_t ModeConfig::getWxPtSensor() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_PT_SENSOR_OFFSET); }
bool ModeConfig::getVictron() const { return readValue<bool>(CONFIG_MODE_OFSET + MODE_VICTRON_OFFSET); }
bool ModeConfig::getDigiViscous() const { return readValue<bool>(CONFIG_MODE_OFSET + MODE_DIGI_VISCOUS_OFFSET); }
bool ModeConfig::getDigiOnlySsid() const { return readValue<bool>(CONFIG_MODE_OFSET + MODE_DIGI_ONLY_SSID_OFFSET); }
uint8_t ModeConfig::getDigiViscousDelay() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_VISCOUS_DELAY_OFFSET); }
uint8_t ModeConfig::getDigiDelay100msec() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_DELAY_100MSEC_OFFSET); }
uint8_t ModeConfig::getPowersave() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_POWERSAVE_OFFSET); }
bool ModeConfig::getPowersaveKeepGsm() const { return readValue<bool>(CONFIG_MODE_OFSET + MODE_POWERSAVE_KEEP_GSM_OFFSET); }
bool ModeConfig::getGsm() const { return readValue<bool>(CONFIG_MODE_OFSET + MODE_GSM_OFFSET); }

void ModeConfig::setDigi(uint8_t digi) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_OFFSET, digi); }
void ModeConfig::setWx(uint8_t wx) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_OFFSET, wx); }
void ModeConfig::setWxUmb(bool wxUmb) { writeValue<bool>(CONFIG_MODE_OFSET + MODE_WX_UMB_OFFSET, wxUmb); }
void ModeConfig::setWxModbus(bool wxModbus) { writeValue<bool>(CONFIG_MODE_OFSET + MODE_WX_MODBUS_OFFSET, wxModbus); }
void ModeConfig::setWxDavis(bool wxDavis) { writeValue<bool>(CONFIG_MODE_OFSET + MODE_WX_DAVIS_OFFSET, wxDavis); }
void ModeConfig::setWxMs5611OrBme(bool sensor) { writeValue<bool>(CONFIG_MODE_OFSET + MODE_WX_MS5611_OR_BME_OFFSET, sensor); }
void ModeConfig::setWxAnemometerConst(uint8_t anemometer) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_ANEMOMETER_CONST_OFFSET, anemometer); }
void ModeConfig::setWxDustSensor(uint8_t dust) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_DUST_SENSOR_OFFSET, dust); }
void ModeConfig::setWxPtSensor(uint8_t ptSensor) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_PT_SENSOR_OFFSET, ptSensor); }
void ModeConfig::setVictron(bool victron) { writeValue<bool>(CONFIG_MODE_OFSET + MODE_VICTRON_OFFSET, victron); }
void ModeConfig::setDigiViscous(bool viscous) { writeValue<bool>(CONFIG_MODE_OFSET + MODE_DIGI_VISCOUS_OFFSET, viscous); }
void ModeConfig::setDigiOnlySsid(bool onlySsid) { writeValue<bool>(CONFIG_MODE_OFSET + MODE_DIGI_ONLY_SSID_OFFSET, onlySsid); }
void ModeConfig::setDigiViscousDelay(uint8_t delay) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_VISCOUS_DELAY_OFFSET, delay); }
void ModeConfig::setDigiDelay100msec(uint8_t delay) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_DELAY_100MSEC_OFFSET, delay); }
void ModeConfig::setPowersave(uint8_t powersave) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_POWERSAVE_OFFSET, powersave); }
void ModeConfig::setPowersaveKeepGsm(bool keepGsm) { writeValue<bool>(CONFIG_MODE_OFSET + MODE_POWERSAVE_KEEP_GSM_OFFSET, keepGsm); }
void ModeConfig::setGsm(bool gsm) { writeValue<bool>(CONFIG_MODE_OFSET + MODE_GSM_OFFSET, gsm); }

// ============================================================================
// SOURCE Configuration Implementation
// ============================================================================

uint8_t SourceConfig::getTemperature() const { return readValue<uint8_t>(CONFIG_SOURCES_OFFSET + SOURCE_TEMPERATURE_OFFSET); }
uint8_t SourceConfig::getPressure() const { return readValue<uint8_t>(CONFIG_SOURCES_OFFSET + SOURCE_PRESSURE_OFFSET); }
uint8_t SourceConfig::getHumidity() const { return readValue<uint8_t>(CONFIG_SOURCES_OFFSET + SOURCE_HUMIDITY_OFFSET); }
uint8_t SourceConfig::getWind() const { return readValue<uint8_t>(CONFIG_SOURCES_OFFSET + SOURCE_WIND_OFFSET); }

void SourceConfig::setTemperature(uint8_t temp) { writeValue<uint8_t>(CONFIG_SOURCES_OFFSET + SOURCE_TEMPERATURE_OFFSET, temp); }
void SourceConfig::setPressure(uint8_t pressure) { writeValue<uint8_t>(CONFIG_SOURCES_OFFSET + SOURCE_PRESSURE_OFFSET, pressure); }
void SourceConfig::setHumidity(uint8_t humidity) { writeValue<uint8_t>(CONFIG_SOURCES_OFFSET + SOURCE_HUMIDITY_OFFSET, humidity); }
void SourceConfig::setWind(uint8_t wind) { writeValue<uint8_t>(CONFIG_SOURCES_OFFSET + SOURCE_WIND_OFFSET, wind); }

// ============================================================================
// UMB Configuration Implementation
// ============================================================================

uint16_t UmbConfig::getSlaveClass() const { return readValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_SLAVE_CLASS_OFFSET); }
uint16_t UmbConfig::getSlaveId() const { return readValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_SLAVE_ID_OFFSET); }
uint16_t UmbConfig::getChannelWindSpeed() const { return readValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDSPEED); }
uint16_t UmbConfig::getChannelWindGust() const { return readValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDGUST); }
uint16_t UmbConfig::getChannelWindDirection() const { return readValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDDIRECTION); }
uint16_t UmbConfig::getChannelTemperature() const { return readValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_CHANNEL_TEMPERATURE); }
uint16_t UmbConfig::getChannelQnh() const { return readValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_CHANNEL_QNH); }

void UmbConfig::setSlaveClass(uint16_t slaveClass) { writeValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_SLAVE_CLASS_OFFSET, slaveClass); }
void UmbConfig::setSlaveId(uint16_t slaveId) { writeValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_SLAVE_ID_OFFSET, slaveId); }
void UmbConfig::setChannelWindSpeed(uint16_t channel) { writeValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDSPEED, channel); }
void UmbConfig::setChannelWindGust(uint16_t channel) { writeValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDGUST, channel); }
void UmbConfig::setChannelWindDirection(uint16_t channel) { writeValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_CHANNEL_WINDDIRECTION, channel); }
void UmbConfig::setChannelTemperature(uint16_t channel) { writeValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_CHANNEL_TEMPERATURE, channel); }
void UmbConfig::setChannelQnh(uint16_t channel) { writeValue<uint16_t>(CONFIG_UMB_OFFSET + UMB_CHANNEL_QNH, channel); }

// ============================================================================
// RTU Configuration Implementation
// ============================================================================

uint16_t RtuConfig::getSlaveSpeed() const { return readValue<uint16_t>(CONFIG_RTU_OFFSET + RTU_SLAVE_SPEED_OFFSET); }
uint8_t RtuConfig::getSlaveParity() const { return readValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_SLAVE_PARITY_SPEED); }
uint8_t RtuConfig::getSlaveStopBits() const { return readValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_SLAVE_STOP_BITS_OFFSET); }
uint8_t RtuConfig::getUseFullWindData() const { return readValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_USE_FULL_WIND_DATA_OFFSET); }
uint8_t RtuConfig::getTemperatureSrc() const { return readValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_TEMPERATURE_SRC_OFFSET); }
uint8_t RtuConfig::getHumiditySrc() const { return readValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_HUMIDITY_SRC_OFFSET); }
uint8_t RtuConfig::getPressureSrc() const { return readValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_PRESSURE_SRC_OFFSET); }
uint8_t RtuConfig::getWindDir() const { return readValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_WIND_DIR_OFFSET); }
uint8_t RtuConfig::getWindSpeed() const { return readValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_WIND_SPEED_OFFSET); }
uint8_t RtuConfig::getWindGusts() const { return readValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_WIND_GUSTS_OFFSET); }

void RtuConfig::setSlaveSpeed(uint16_t speed) { writeValue<uint16_t>(CONFIG_RTU_OFFSET + RTU_SLAVE_SPEED_OFFSET, speed); }
void RtuConfig::setSlaveParity(uint8_t parity) { writeValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_SLAVE_PARITY_SPEED, parity); }
void RtuConfig::setSlaveStopBits(uint8_t stopBits) { writeValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_SLAVE_STOP_BITS_OFFSET, stopBits); }
void RtuConfig::setUseFullWindData(uint8_t useFullWind) { writeValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_USE_FULL_WIND_DATA_OFFSET, useFullWind); }
void RtuConfig::setTemperatureSrc(uint8_t tempSrc) { writeValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_TEMPERATURE_SRC_OFFSET, tempSrc); }
void RtuConfig::setHumiditySrc(uint8_t humiditySrc) { writeValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_HUMIDITY_SRC_OFFSET, humiditySrc); }
void RtuConfig::setPressureSrc(uint8_t pressureSrc) { writeValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_PRESSURE_SRC_OFFSET, pressureSrc); }
void RtuConfig::setWindDir(uint8_t windDir) { writeValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_WIND_DIR_OFFSET, windDir); }
void RtuConfig::setWindSpeed(uint8_t windSpeed) { writeValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_WIND_SPEED_OFFSET, windSpeed); }
void RtuConfig::setWindGusts(uint8_t windGusts) { writeValue<uint8_t>(CONFIG_RTU_OFFSET + RTU_WIND_GUSTS_OFFSET, windGusts); }

size_t RtuConfig::howManySlaves () const
{
	return RTU_SLAVE_HOW_MANY;
}

void RtuConfig::setSlave (uint8_t id, RtuSlave &data)
{
	// indexing starts at zero
	if (id >= RTU_SLAVE_HOW_MANY)
	{
		throw std::out_of_range("Try to set RTU slave with ID bigger than maximum!");
	}

	size_t offset = CONFIG_RTU_OFFSET + RTU_SLAVE_CONFIG_BLOCK_OFFSET + (RTU_SLAVE_CONFIG_BLOCK_SIZE * id);

	writeValue<uint8_t>(offset + RTU_X_BUS_ADDRESS, data.busAddress);
	writeValue<uint8_t>(offset + RTU_X_FUNCTION, data.function);
	writeValue<uint16_t>(offset + RTU_X_REGUSTER_ADDR, data.registerAddress);
	writeValue<uint16_t>(offset + RTU_X_LENGHT, data.readLen);
	writeValue<uint8_t>(offset + RTU_X_SCALLING_A, data.scalingA);
	writeValue<uint8_t>(offset + RTU_X_SCALLING_B, data.scalingB);
	writeValue<uint8_t>(offset + RTU_X_SCALLING_C, data.scalingC);
	writeValue<uint8_t>(offset + RTU_X_SCALLING_D, data.scalingD);
	writeValue<uint8_t>(offset + RTU_X_UNSIGNED_SIGNED, (data.signedValue == true) ? 1 : 0);

}

RtuSlave RtuConfig::getSlave (uint8_t id) const
{
	// indexing starts at zero
	if (id >= RTU_SLAVE_HOW_MANY)
	{
		throw std::out_of_range("Try to request RTU slave with ID bigger than maximum!");
	}

	size_t offset = CONFIG_RTU_OFFSET + RTU_SLAVE_CONFIG_BLOCK_OFFSET + (RTU_SLAVE_CONFIG_BLOCK_SIZE * id);

	RtuSlave out = {0u};

	out.busAddress = readValue<uint8_t>(offset + RTU_X_BUS_ADDRESS);
	out.function = readValue<uint8_t>(offset + RTU_X_FUNCTION);
	out.readLen = readValue<uint16_t>(offset + RTU_X_LENGHT);
	out.registerAddress = readValue<uint16_t>(offset +  RTU_X_REGUSTER_ADDR);
	out.scalingA = readValue<uint8_t>(offset + RTU_X_SCALLING_A);
	out.scalingB = readValue<uint8_t>(offset + RTU_X_SCALLING_B);
	out.scalingC = readValue<uint8_t>(offset + RTU_X_SCALLING_C);
	out.scalingD = readValue<uint8_t>(offset + RTU_X_SCALLING_D);

	const uint8_t unsignedraw = readValue<uint8_t>(offset + RTU_X_UNSIGNED_SIGNED);

	if (unsignedraw == 0)
	{
		out.signedValue = false;
	}
	else
	{
		out.signedValue = true;
	}

	return out;
}

// ============================================================================
// GSM Configuration Implementation
// ============================================================================

std::string GsmConfig::readString(size_t offset, size_t maxLength) const {
    if (offset + maxLength > configData.size()) {
        throw std::out_of_range("Config data access out of bounds");
    }
    std::string result;
    for (size_t i = 0; i < maxLength; ++i) {
        uint8_t byte = configData[offset + i];
        if (byte == 0) break;
        result += static_cast<char>(byte);
    }
    return result;
}

void GsmConfig::writeString(size_t offset, size_t maxLength, const std::string& value) {
    if (offset + maxLength > configData.size()) {
        throw std::out_of_range("Config data access out of bounds");
    }
    if (value.length() >= maxLength) {
        throw std::invalid_argument("String too long for field");
    }
    auto& mutableData = const_cast<std::vector<uint8_t>&>(configData);
    std::memcpy(&mutableData[offset], value.c_str(), value.length());
    mutableData[offset + value.length()] = 0;
    for (size_t i = value.length() + 1; i < maxLength; ++i) {
        mutableData[offset + i] = 0;
    }
}

void GsmConfig::getPin(std::string& pin) const { pin = readString(CONFIG_GSM_OFFSET + GSM_PIN_OFFSET, GSM_PIN_LENGHT); }
std::string GsmConfig::getPin() const { return readString(CONFIG_GSM_OFFSET + GSM_PIN_OFFSET, GSM_PIN_LENGHT); }
void GsmConfig::getApn(std::string& apn) const { apn = readString(CONFIG_GSM_OFFSET + GSM_APN_OFFSET, GSM_APN_LENGHT); }
std::string GsmConfig::getApn() const { return readString(CONFIG_GSM_OFFSET + GSM_APN_OFFSET, GSM_APN_LENGHT); }
void GsmConfig::getUsername(std::string& username) const { username = readString(CONFIG_GSM_OFFSET + GSM_USERNAME_OFFSET, GSM_USERNAME_LEN); }
std::string GsmConfig::getUsername() const { return readString(CONFIG_GSM_OFFSET + GSM_USERNAME_OFFSET, GSM_USERNAME_LEN); }
void GsmConfig::getPassword(std::string& password) const { password = readString(CONFIG_GSM_OFFSET + GSM_PASSWORD_OFFSET, GSM_PASSWORD_LEN); }
std::string GsmConfig::getPassword() const { return readString(CONFIG_GSM_OFFSET + GSM_PASSWORD_OFFSET, GSM_PASSWORD_LEN); }
uint8_t GsmConfig::getApiEnable() const { return readValue<uint8_t>(CONFIG_GSM_OFFSET + GSM_API_ENABLE_OFFSET); }
void GsmConfig::getApiBaseUrl(std::string& url) const { url = readString(CONFIG_GSM_OFFSET + GSM_API_BASE_URL_OFFSET, GSM_API_BASE_URL_LEN); }
std::string GsmConfig::getApiBaseUrl() const { return readString(CONFIG_GSM_OFFSET + GSM_API_BASE_URL_OFFSET, GSM_API_BASE_URL_LEN); }
void GsmConfig::getApiStationName(std::string& stationName) const { stationName = readString(CONFIG_GSM_OFFSET + GSM_API_STATION_NAME_OFFSET, GSM_API_STATION_NAME_LN); }
std::string GsmConfig::getApiStationName() const { return readString(CONFIG_GSM_OFFSET + GSM_API_STATION_NAME_OFFSET, GSM_API_STATION_NAME_LN); }
uint8_t GsmConfig::getAprsisEnable() const { return readValue<uint8_t>(CONFIG_GSM_OFFSET + GSM_APRSIS_ENABLE); }
void GsmConfig::getAprsisServer(std::string& server) const { server = readString(CONFIG_GSM_OFFSET + GSM_APRSIS_SERVER_OFFSET, GSM_APRSIS_SERVER_LN); }
std::string GsmConfig::getAprsisServer() const { return readString(CONFIG_GSM_OFFSET + GSM_APRSIS_SERVER_OFFSET, GSM_APRSIS_SERVER_LN); }
uint16_t GsmConfig::getAprsisPort() const { return readValue<uint16_t>(CONFIG_GSM_OFFSET + GSM_APRSIS_PORT_OFFSET); }
void GsmConfig::getAprsisPpasscode(std::string& passcode) const { passcode = readString(CONFIG_GSM_OFFSET + GSM_APRSIS_PASSCODE_OFFSET, 8); }
std::string GsmConfig::getAprsisPpasscode() const { return readString(CONFIG_GSM_OFFSET + GSM_APRSIS_PASSCODE_OFFSET, 8); }

void GsmConfig::setPin(const std::string& pin) { writeString(CONFIG_GSM_OFFSET + GSM_PIN_OFFSET, GSM_PIN_LENGHT, pin); }
void GsmConfig::setApn(const std::string& apn) { writeString(CONFIG_GSM_OFFSET + GSM_APN_OFFSET, GSM_APN_LENGHT, apn); }
void GsmConfig::setUsername(const std::string& username) { writeString(CONFIG_GSM_OFFSET + GSM_USERNAME_OFFSET, GSM_USERNAME_LEN, username); }
void GsmConfig::setPassword(const std::string& password) { writeString(CONFIG_GSM_OFFSET + GSM_PASSWORD_OFFSET, GSM_PASSWORD_LEN, password); }
void GsmConfig::setApiEnable(uint8_t apiEnable) { writeValue<uint8_t>(CONFIG_GSM_OFFSET + GSM_API_ENABLE_OFFSET, apiEnable); }
void GsmConfig::setApiBaseUrl(const std::string& url) { writeString(CONFIG_GSM_OFFSET + GSM_API_BASE_URL_OFFSET, GSM_API_BASE_URL_LEN, url); }
void GsmConfig::setApiStationName(const std::string& stationName) { writeString(CONFIG_GSM_OFFSET + GSM_API_STATION_NAME_OFFSET, GSM_API_STATION_NAME_LN, stationName); }
void GsmConfig::setAprsisEnable(uint8_t aprsisEnable) { writeValue<uint8_t>(CONFIG_GSM_OFFSET + GSM_APRSIS_ENABLE, aprsisEnable); }
void GsmConfig::setAprsisServer(const std::string& server) { writeString(CONFIG_GSM_OFFSET + GSM_APRSIS_SERVER_OFFSET, GSM_APRSIS_SERVER_LN, server); }
void GsmConfig::setAprsisPort(uint16_t port) { writeValue<uint16_t>(CONFIG_GSM_OFFSET + GSM_APRSIS_PORT_OFFSET, port); }
void GsmConfig::setAprsisPpasscode(const std::string& passcode) { writeString(CONFIG_GSM_OFFSET + GSM_APRSIS_PASSCODE_OFFSET, 8, passcode); }
// clang-format on

// ============================================================================
//
// ============================================================================

ConfigurationManager::ConfigurationManager ()
	: configData (CONFIG_BLOCK_SIZE, 0), basicConfig (configData), modeConfig (configData),
	  sourceConfig (configData), umbConfig (configData), rtuConfig (configData),
	  gsmConfig (configData)
{
}

uint32_t ConfigurationManager::getConfigCounter ()
{
	if (configData.size () >= CONFIG_BLOCK_SIZE) {
		uint32_t *ptr = (uint32_t *)(configData.data () + CONFIG_MODE_PGM_CNTR);
		return *ptr;
	}
	else {
		throw std::out_of_range (
			"Config data not allocated while trying to get CONFIG_MODE_PGM_CNTR");
	}
}

void ConfigurationManager::setConfigCounter (uint32_t value)
{
	if (configData.size () >= CONFIG_BLOCK_SIZE) {
		uint32_t *ptr = (uint32_t *)(configData.data () + CONFIG_MODE_PGM_CNTR);
		*ptr = value;
	}
	else {
		throw std::out_of_range (
			"Config data not allocated while trying to set CONFIG_MODE_PGM_CNTR");
	}
}

uint32_t ConfigurationManager::calculateAndSetChecksum ()
{
	ValidateVer0 validate;

	if (configData.size () < CONFIG_BLOCK_SIZE) {
		throw std::runtime_error (
			"Configuration data block size is too small to be valid and calculate CRC for");
	}

	// recalculate CRC checksum for current configuration block
	const bool crcCalcRes = validate.recalculateChecksum (configData);

	if (!crcCalcRes) {
		throw std::runtime_error ("Failure during CRC32 calculation in ConfigurationManager!");
	}

	uint32_t crcAreaLn = (uint32_t)(CONFIG_BLOCK_SIZE - 12);

	// data are stored in litte endian order
	uint32_t crcFromFrame = (configData.at (crcAreaLn + 4)) | (configData.at (crcAreaLn + 5) << 8) |
							(configData.at (crcAreaLn + 6) << 16) |
							(configData.at (crcAreaLn + 7) << 24);

	// use the same validation, that is used during uploading configuration controller -> PC
	const bool crcRevalidate = validate.checkValidate (configData);

	if (!crcRevalidate) {
		throw std::runtime_error ("!crcCalcRes");
	}

	return crcFromFrame;
}

void ConfigurationManager::print (PrintVerbosity verbosity)
{
	std::cout << "====================================" << std::endl;
	if (verbosity == PrintVerbosity::BRIEF_SUMMARY) {
		std::cout << "=====Configuration BRIEF SUMMARY====" << std::endl;

		std::cout << "C = station APRS callsign: " << getBasicConfig ().getCallsign ();
		if (getBasicConfig ().getSsid () != 0) {
			std::cout << "-" << getBasicConfig ().getSsid () << std::endl;
		}
		else {
			std::cout << std::endl;
		}
		std::cout << "C = API station name: " << getGsmConfig ().getApiStationName () << std::endl;
		std::cout << "C = APRS station description: " << getBasicConfig ().getComment ()
				  << std::endl;
		std::cout << "C = weather packets interval: "
				  << (int)getBasicConfig ().getWxTransmitPeriod () << " mins" << std::endl;
		std::cout << "C = beacon packets interval: "
				  << (int)getBasicConfig ().getBeaconTransmitPeriod () << " mins" << std::endl;
		std::cout << "C = digipeater delay: " << getModeConfig ().getDigiDelay100msec () * 100
				  << " ms" << std::endl;
	}
	std::cout << "====================================" << std::endl;
}
