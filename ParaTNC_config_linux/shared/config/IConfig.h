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
    virtual std::string getCallsign() const = 0;
    virtual uint8_t getSsid() const = 0;
    virtual float getLatitude() const = 0;
    virtual uint8_t getNs() const = 0;
    virtual float getLongitude() const = 0;
    virtual uint8_t getWe() const = 0;
    virtual void getComment(std::string& comment) const = 0;
    virtual std::string getComment() const = 0;
    virtual uint8_t getSymbol() const = 0;
    virtual uint8_t getPathType() const = 0;
    virtual bool getBeaconBootup() const = 0;
    virtual uint8_t getWxTransmitPeriod() const = 0;
    virtual uint8_t getBeaconTransmitPeriod() const = 0;
    virtual bool getWxDoubleTransmit() const = 0;

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
    virtual void setBeaconBootup(bool bootup) = 0;
    virtual void setWxTransmitPeriod(uint8_t period) = 0;
    virtual void setBeaconTransmitPeriod(uint8_t period) = 0;
    virtual void setWxDoubleTransmit(bool doubleTransmit) = 0;
};

// ============================================================================
// MODE Configuration Decoder/Encoder
// ============================================================================
class IModeConfig {
public:
    virtual ~IModeConfig();

    virtual uint8_t getDigi() const = 0;
    virtual uint8_t getWx() const = 0;
    virtual bool getWxUmb() const = 0;
    virtual bool getWxModbus() const = 0;
    virtual bool getWxDavis() const = 0;
    virtual bool getWxMs5611OrBme() const = 0;
    virtual uint8_t getWxAnemometerConst() const = 0;
    virtual uint8_t getWxDustSensor() const = 0;
    virtual uint8_t getWxPtSensor() const = 0;
    virtual bool getVictron() const = 0;
    virtual bool getDigiViscous() const = 0;
    virtual bool getDigiOnlySsid() const = 0;
    virtual uint8_t getDigiViscousDelay() const = 0;
    virtual uint8_t getDigiDelay100msec() const = 0;
    virtual uint8_t getPowersave() const = 0;
    virtual bool getPowersaveKeepGsm() const = 0;
    virtual bool getGsm() const = 0;

    virtual void setDigi(uint8_t digi) = 0;
    virtual void setWx(uint8_t wx) = 0;
    virtual void setWxUmb(bool wxUmb) = 0;
    virtual void setWxModbus(bool wxModbus) = 0;
    virtual void setWxDavis(bool wxDavis) = 0;
    virtual void setWxMs5611OrBme(bool sensor) = 0;
    virtual void setWxAnemometerConst(uint8_t anemometer) = 0;
    virtual void setWxDustSensor(uint8_t dust) = 0;
    virtual void setWxPtSensor(uint8_t ptSensor) = 0;
    virtual void setVictron(bool victron) = 0;
    virtual void setDigiViscous(bool viscous) = 0;
    virtual void setDigiOnlySsid(bool onlySsid) = 0;
    virtual void setDigiViscousDelay(uint8_t delay) = 0;
    virtual void setDigiDelay100msec(uint8_t delay) = 0;
    virtual void setPowersave(uint8_t powersave) = 0;
    virtual void setPowersaveKeepGsm(bool keepGsm) = 0;
    virtual void setGsm(bool gsm) = 0;
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
    virtual ~IGsmConfig();

    virtual void getPin(std::string& pin) const = 0;
    virtual std::string getPin() const = 0;
    virtual void getApn(std::string& apn) const = 0;
    virtual std::string getApn() const = 0;
    virtual void getUsername(std::string& username) const = 0;
    virtual std::string getUsername() const = 0;
    virtual void getPassword(std::string& password) const = 0;
    virtual std::string getPassword() const = 0;
    virtual uint8_t getApiEnable() const = 0;
    virtual void getApiBaseUrl(std::string& url) const = 0;
    virtual std::string getApiBaseUrl() const = 0;
    virtual void getApiStationName(std::string& stationName) const = 0;
    virtual std::string getApiStationName() const = 0;
    virtual uint8_t getAprsisEnable() const = 0;
    virtual void getAprsisServer(std::string& server) const = 0;
    virtual std::string getAprsisServer() const = 0;
    virtual uint16_t getAprsisPort() const = 0;
    virtual void getAprsisPpasscode(std::string& passcode) const = 0;
    virtual std::string getAprsisPpasscode() const = 0;

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

	enum class PrintVerbosity
	{
		BASIC,
		MODE,
		SOURCE,
		UMB,
		RTU,
		GSM,
		BRIEF_SUMMARY,
		FULL
	};

	virtual ~IConfigurationManager();

    virtual IBasicConfig& getBasicConfig() = 0;
    virtual IModeConfig& getModeConfig() = 0;
    virtual ISourceConfig& getSourceConfig() = 0;
    virtual IUmbConfig& getUmbConfig() = 0;
    virtual IRtuConfig& getRtuConfig() = 0;
    virtual IGsmConfig& getGsmConfig() = 0;

    virtual const std::vector<uint8_t>& getConfigData() const = 0;

    virtual void print(PrintVerbosity verbosity) = 0;
};

#endif /* SRC_CONFIG_DECODE_ICONFIGDECODE_H_ */
