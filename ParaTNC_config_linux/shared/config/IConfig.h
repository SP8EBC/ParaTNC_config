/*
 * IConfigDecode.h
 *
 *  Created on: Aug 27, 2022
 *      Author: mateusz
 */

#ifndef SRC_CONFIG_DECODE_ICONFIGDECODE_H_
#define SRC_CONFIG_DECODE_ICONFIGDECODE_H_

#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include <stdexcept>

// ============================================================================
// BASIC Configuration Decoder/Encoder
// ============================================================================
class IBasicConfig {

public:
    virtual ~IBasicConfig();

    // Decoder methods
    virtual void getCallsign(std::string& call) const = 0;
    virtual uint8_t getSsid() const = 0;
    virtual float getLatitude() const = 0;
    virtual uint8_t getNs() const = 0;
    virtual float getLongitude() const = 0;
    virtual uint8_t getWe() const = 0;
    virtual void getComment(std::string& comment) const = 0;
    virtual uint8_t getSymbol() const = 0;
    virtual uint8_t getPathType() const = 0;
    virtual uint8_t getBeaconBootup() const = 0;
    virtual uint8_t getWxTransmitPeriod() const = 0;
    virtual uint8_t getBeaconTransmitPeriod() const = 0;
    virtual uint8_t getWxDoubleTransmit() const = 0;

    // Encoder methods
    virtual void setCallsign(const std::string& call) = 0;
    virtual void setSsid(uint8_t ssid) = 0;
    virtual void setLatitude(float latitude) = 0;
    virtual void setNs(uint8_t ns) = 0;
    virtual void setLongitude(float longitude) = 0;
    virtual void setWe(uint8_t we) = 0;
    virtual void setComment(const std::string& comment) = 0;
    virtual void setSymbol(uint8_t symbol) = 0;
    virtual void setPathType(uint8_t pathType) = 0;
    virtual void setBeaconBootup(uint8_t bootup) = 0;
    virtual void setWxTransmitPeriod(uint8_t period) = 0;
    virtual void setBeaconTransmitPeriod(uint8_t period) = 0;
    virtual void setWxDoubleTransmit(uint8_t doubleTransmit) = 0;
};

// ============================================================================
// MODE Configuration Decoder/Encoder
// ============================================================================
class IModeConfig {
public:
    virtual ~IModeConfig();

    virtual uint8_t getDigi() const = 0;
    virtual uint8_t getWx() const = 0;
    virtual uint8_t getWxUmb() const = 0;
    virtual uint8_t getWxModbus() const = 0;
    virtual uint8_t getWxDavis() const = 0;
    virtual uint8_t getWxMs5611OrBme() const = 0;
    virtual uint8_t getWxAnemometerConst() const = 0;
    virtual uint8_t getWxDustSensor() const = 0;
    virtual uint8_t getWxPtSensor() const = 0;
    virtual uint8_t getVictron() const = 0;
    virtual uint8_t getDigiViscous() const = 0;
    virtual uint8_t getDigiOnlySsid() const = 0;
    virtual uint8_t getDigiViscousDelay() const = 0;
    virtual uint8_t getDigiDelay100msec() const = 0;
    virtual uint8_t getPowersave() const = 0;
    virtual uint8_t getPowersaveKeepGsm() const = 0;
    virtual uint8_t getGsm() const = 0;

    virtual void setDigi(uint8_t digi) = 0;
    virtual void setWx(uint8_t wx) = 0;
    virtual void setWxUmb(uint8_t wxUmb) = 0;
    virtual void setWxModbus(uint8_t wxModbus) = 0;
    virtual void setWxDavis(uint8_t wxDavis) = 0;
    virtual void setWxMs5611OrBme(uint8_t sensor) = 0;
    virtual void setWxAnemometerConst(uint8_t anemometer) = 0;
    virtual void setWxDustSensor(uint8_t dust) = 0;
    virtual void setWxPtSensor(uint8_t ptSensor) = 0;
    virtual void setVictron(uint8_t victron) = 0;
    virtual void setDigiViscous(uint8_t viscous) = 0;
    virtual void setDigiOnlySsid(uint8_t onlySsid) = 0;
    virtual void setDigiViscousDelay(uint8_t delay) = 0;
    virtual void setDigiDelay100msec(uint8_t delay) = 0;
    virtual void setPowersave(uint8_t powersave) = 0;
    virtual void setPowersaveKeepGsm(uint8_t keepGsm) = 0;
    virtual void setGsm(uint8_t gsm) = 0;
};

// ============================================================================
// SOURCE Configuration Decoder/Encoder
// ============================================================================
class ISourceConfig {

public:
    virtual ~ISourceConfig();

    virtual uint8_t getTemperature() const = 0;
    virtual uint8_t getPressure() const = 0;
    virtual uint8_t getHumidity() const = 0;
    virtual uint8_t getWind() const = 0;

    virtual void setTemperature(uint8_t temp) = 0;
    virtual void setPressure(uint8_t pressure) = 0;
    virtual void setHumidity(uint8_t humidity) = 0;
    virtual void setWind(uint8_t wind) = 0;
};

// ============================================================================
// UMB Configuration Decoder/Encoder
// ============================================================================
class IUmbConfig {
public:
    virtual ~IUmbConfig();

    virtual uint16_t getSlaveClass() const = 0;
    virtual uint16_t getSlaveId() const = 0;
    virtual uint16_t getChannelWindSpeed() const = 0;
    virtual uint16_t getChannelWindGust() const = 0;
    virtual uint16_t getChannelWindDirection() const = 0;
    virtual uint16_t getChannelTemperature() const = 0;
    virtual uint16_t getChannelQnh() const = 0;

    virtual void setSlaveClass(uint16_t slaveClass) = 0;
    virtual void setSlaveId(uint16_t slaveId) = 0;
    virtual void setChannelWindSpeed(uint16_t channel) = 0;
    virtual void setChannelWindGust(uint16_t channel) = 0;
    virtual void setChannelWindDirection(uint16_t channel) = 0;
    virtual void setChannelTemperature(uint16_t channel) = 0;
    virtual void setChannelQnh(uint16_t channel) = 0;
};

// ============================================================================
// RTU Configuration Decoder/Encoder
// ============================================================================
class IRtuConfig {
public:
    virtual ~IRtuConfig();

    virtual uint16_t getSlaveSpeed() const = 0;
    virtual uint8_t getSlaveParity() const = 0;
    virtual uint8_t getSlaveStopBits() const = 0;
    virtual uint8_t getUseFullWindData() const = 0;
    virtual uint8_t getTemperatureSrc() const = 0;
    virtual uint8_t getHumiditySrc() const = 0;
    virtual uint8_t getPressureSrc() const = 0;
    virtual uint8_t getWindDir() const = 0;
    virtual uint8_t getWindSpeed() const = 0;
    virtual uint8_t getWindGusts() const = 0;

    virtual void setSlaveSpeed(uint16_t speed) = 0;
    virtual void setSlaveParity(uint8_t parity) = 0;
    virtual void setSlaveStopBits(uint8_t stopBits) = 0;
    virtual void setUseFullWindData(uint8_t useFullWind) = 0;
    virtual void setTemperatureSrc(uint8_t tempSrc) = 0;
    virtual void setHumiditySrc(uint8_t humiditySrc) = 0;
    virtual void setPressureSrc(uint8_t pressureSrc) = 0;
    virtual void setWindDir(uint8_t windDir) = 0;
    virtual void setWindSpeed(uint8_t windSpeed) = 0;
    virtual void setWindGusts(uint8_t windGusts) = 0;
};

// ============================================================================
// GSM Configuration Decoder/Encoder
// ============================================================================
class IGsmConfig {

public:
    virtual ~IGsmConfig() = default;

    virtual void getPin(std::string& pin) const = 0;
    virtual void getApn(std::string& apn) const = 0;
    virtual void getUsername(std::string& username) const = 0;
    virtual void getPassword(std::string& password) const = 0;
    virtual uint8_t getApiEnable() const = 0;
    virtual void getApiBaseUrl(std::string& url) const = 0;
    virtual void getApiStationName(std::string& stationName) const = 0;
    virtual uint8_t getAprsisEnable() const = 0;
    virtual void getAprsisServer(std::string& server) const = 0;
    virtual uint16_t getAprsisPort() const = 0;
    virtual void getAprsisPpasscode(std::string& passcode) const = 0;

    virtual void setPin(const std::string& pin) = 0;
    virtual void setApn(const std::string& apn) = 0;
    virtual void setUsername(const std::string& username) = 0;
    virtual void setPassword(const std::string& password) = 0;
    virtual void setApiEnable(uint8_t apiEnable) = 0;
    virtual void setApiBaseUrl(const std::string& url) = 0;
    virtual void setApiStationName(const std::string& stationName) = 0;
    virtual void setAprsisEnable(uint8_t aprsisEnable) = 0;
    virtual void setAprsisServer(const std::string& server) = 0;
    virtual void setAprsisPort(uint16_t port) = 0;
    virtual void setAprsisPpasscode(const std::string& passcode) = 0;
};

// ============================================================================
// Main Configuration Manager
// ============================================================================
class IConfigurationManager {

public:

    IBasicConfig& getBasicConfig() = 0;
    IModeConfig& getModeConfig() = 0;
    ISourceConfig& getSourceConfig() = 0;
    IUmbConfig& getUmbConfig() = 0;
    IRtuConfig& getRtuConfig() = 0;
    IGsmConfig& getGsmConfig() = 0;

    const std::vector<uint8_t>& getConfigData() const = 0;
};

#endif /* SRC_CONFIG_DECODE_ICONFIGDECODE_H_ */
