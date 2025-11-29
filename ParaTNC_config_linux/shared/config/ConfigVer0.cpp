/*
 * ConfigVer0.cpp
 *
 *  Created on: Nov 18, 2025
 *      Author: mateusz
 */

#include "ConfigVer0.h"
#include "Ver0Map.h"

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

uint8_t BasicConfig::getSymbol() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_SYMBOL_OFFSET);
}

uint8_t BasicConfig::getPathType() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_PATHTYPE_OFFSET);
}

uint8_t BasicConfig::getBeaconBootup() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_BEACON_BOOTUP_OFFSET);
}

uint8_t BasicConfig::getWxTransmitPeriod() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_WX_TRANSMIT_PERIOD);
}

uint8_t BasicConfig::getBeaconTransmitPeriod() const {
    return readValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_BEACON_TRANSMIT_PERIOD);
}

uint8_t BasicConfig::getWxDoubleTransmit() const {
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

void BasicConfig::setBeaconBootup(uint8_t bootup) {
    writeValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_BEACON_BOOTUP_OFFSET, bootup);
}

void BasicConfig::setWxTransmitPeriod(uint8_t period) {
    writeValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_WX_TRANSMIT_PERIOD, period);
}

void BasicConfig::setBeaconTransmitPeriod(uint8_t period) {
    writeValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_BEACON_TRANSMIT_PERIOD, period);
}

void BasicConfig::setWxDoubleTransmit(uint8_t doubleTransmit) {
    writeValue<uint8_t>(CONFIG_BASIC_OFFSET + BASIC_WX_DOUBLE_TRANSMIT, doubleTransmit);
}

// ============================================================================
// MODE Configuration Implementation
// ============================================================================

uint8_t ModeConfig::getDigi() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_OFFSET); }
uint8_t ModeConfig::getWx() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_OFFSET); }
uint8_t ModeConfig::getWxUmb() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_UMB_OFFSET); }
uint8_t ModeConfig::getWxModbus() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_MODBUS_OFFSET); }
uint8_t ModeConfig::getWxDavis() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_DAVIS_OFFSET); }
uint8_t ModeConfig::getWxMs5611OrBme() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_MS5611_OR_BME_OFFSET); }
uint8_t ModeConfig::getWxAnemometerConst() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_ANEMOMETER_CONST_OFFSET); }
uint8_t ModeConfig::getWxDustSensor() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_DUST_SENSOR_OFFSET); }
uint8_t ModeConfig::getWxPtSensor() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_PT_SENSOR_OFFSET); }
uint8_t ModeConfig::getVictron() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_VICTRON_OFFSET); }
uint8_t ModeConfig::getDigiViscous() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_VISCOUS_OFFSET); }
uint8_t ModeConfig::getDigiOnlySsid() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_ONLY_SSID_OFFSET); }
uint8_t ModeConfig::getDigiViscousDelay() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_VISCOUS_DELAY_OFFSET); }
uint8_t ModeConfig::getDigiDelay100msec() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_DELAY_100MSEC_OFFSET); }
uint8_t ModeConfig::getPowersave() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_POWERSAVE_OFFSET); }
uint8_t ModeConfig::getPowersaveKeepGsm() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_POWERSAVE_KEEP_GSM_OFFSET); }
uint8_t ModeConfig::getGsm() const { return readValue<uint8_t>(CONFIG_MODE_OFSET + MODE_GSM_OFFSET); }

void ModeConfig::setDigi(uint8_t digi) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_OFFSET, digi); }
void ModeConfig::setWx(uint8_t wx) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_OFFSET, wx); }
void ModeConfig::setWxUmb(uint8_t wxUmb) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_UMB_OFFSET, wxUmb); }
void ModeConfig::setWxModbus(uint8_t wxModbus) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_MODBUS_OFFSET, wxModbus); }
void ModeConfig::setWxDavis(uint8_t wxDavis) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_DAVIS_OFFSET, wxDavis); }
void ModeConfig::setWxMs5611OrBme(uint8_t sensor) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_MS5611_OR_BME_OFFSET, sensor); }
void ModeConfig::setWxAnemometerConst(uint8_t anemometer) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_ANEMOMETER_CONST_OFFSET, anemometer); }
void ModeConfig::setWxDustSensor(uint8_t dust) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_DUST_SENSOR_OFFSET, dust); }
void ModeConfig::setWxPtSensor(uint8_t ptSensor) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_WX_PT_SENSOR_OFFSET, ptSensor); }
void ModeConfig::setVictron(uint8_t victron) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_VICTRON_OFFSET, victron); }
void ModeConfig::setDigiViscous(uint8_t viscous) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_VISCOUS_OFFSET, viscous); }
void ModeConfig::setDigiOnlySsid(uint8_t onlySsid) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_ONLY_SSID_OFFSET, onlySsid); }
void ModeConfig::setDigiViscousDelay(uint8_t delay) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_VISCOUS_DELAY_OFFSET, delay); }
void ModeConfig::setDigiDelay100msec(uint8_t delay) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_DIGI_DELAY_100MSEC_OFFSET, delay); }
void ModeConfig::setPowersave(uint8_t powersave) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_POWERSAVE_OFFSET, powersave); }
void ModeConfig::setPowersaveKeepGsm(uint8_t keepGsm) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_POWERSAVE_KEEP_GSM_OFFSET, keepGsm); }
void ModeConfig::setGsm(uint8_t gsm) { writeValue<uint8_t>(CONFIG_MODE_OFSET + MODE_GSM_OFFSET, gsm); }

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
void GsmConfig::getApn(std::string& apn) const { apn = readString(CONFIG_GSM_OFFSET + GSM_APN_OFFSET, GSM_APN_LENGHT); }
void GsmConfig::getUsername(std::string& username) const { username = readString(CONFIG_GSM_OFFSET + GSM_USERNAME_OFFSET, GSM_USERNAME_LEN); }
void GsmConfig::getPassword(std::string& password) const { password = readString(CONFIG_GSM_OFFSET + GSM_PASSWORD_OFFSET, GSM_PASSWORD_LEN); }
uint8_t GsmConfig::getApiEnable() const { return readValue<uint8_t>(CONFIG_GSM_OFFSET + GSM_API_ENABLE_OFFSET); }
void GsmConfig::getApiBaseUrl(std::string& url) const { url = readString(CONFIG_GSM_OFFSET + GSM_API_BASE_URL_OFFSET, GSM_API_BASE_URL_LEN); }
void GsmConfig::getApiStationName(std::string& stationName) const { stationName = readString(CONFIG_GSM_OFFSET + GSM_API_STATION_NAME_OFFSET, GSM_API_STATION_NAME_LN); }
uint8_t GsmConfig::getAprsisEnable() const { return readValue<uint8_t>(CONFIG_GSM_OFFSET + GSM_APRSIS_ENABLE); }
void GsmConfig::getAprsisServer(std::string& server) const { server = readString(CONFIG_GSM_OFFSET + GSM_APRSIS_SERVER_OFFSET, GSM_APRSIS_SERVER_LN); }
uint16_t GsmConfig::getAprsisPort() const { return readValue<uint16_t>(CONFIG_GSM_OFFSET + GSM_APRSIS_PORT_OFFSET); }
void GsmConfig::getAprsisPpasscode(std::string& passcode) const { passcode = readString(CONFIG_GSM_OFFSET + GSM_APRSIS_PASSCODE_OFFSET, 8); }

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
