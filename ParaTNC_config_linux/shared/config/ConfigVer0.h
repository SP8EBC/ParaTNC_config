/*
 * ConfigVer0.h
 *
 *  Created on: Nov 18, 2025
 *      Author: mateusz
 */

#ifndef SHARED_CONFIG_CONFIGVER0_H_
#define SHARED_CONFIG_CONFIGVER0_H_


#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include <stdexcept>

#include "IConfig.h"

// ============================================================================
// BASIC Configuration Decoder/Encoder
// ============================================================================
class BasicConfig : public IBasicConfig {
protected:
    const std::vector<uint8_t>& configData;

    // Helper methods
    std::string readString(size_t offset, size_t maxLength) const;
    void writeString(size_t offset, size_t maxLength, const std::string& value);

    template<typename T>
    T readValue(size_t offset) const {
        if (offset + sizeof(T) > configData.size()) {
            throw std::out_of_range("Config data access out of bounds");
        }
        T value;
        std::memcpy(&value, &configData[offset], sizeof(T));
        return value;
    }

    template<typename T>
    void writeValue(size_t offset, T value) {
        if (offset + sizeof(T) > configData.size()) {
            throw std::out_of_range("Config data access out of bounds");
        }
        std::memcpy(&const_cast<std::vector<uint8_t>&>(configData)[offset], &value, sizeof(T));
    }

public:
    BasicConfig(const std::vector<uint8_t>& data);
    virtual ~BasicConfig() = default;

    // Decoder methods
    virtual void getCallsign(std::string& call) const;
    virtual std::string getCallsign() const;
    virtual uint8_t getSsid() const;
    virtual float getLatitude() const;
    virtual uint8_t getNs() const;
    virtual float getLongitude() const;
    virtual uint8_t getWe() const;
    virtual void getComment(std::string& comment) const;
    virtual std::string getComment() const;
    virtual uint8_t getSymbol() const;
    virtual uint8_t getPathType() const;
    virtual bool getBeaconBootup() const;
    virtual uint8_t getWxTransmitPeriod() const;
    virtual uint8_t getBeaconTransmitPeriod() const;
    virtual bool getWxDoubleTransmit() const;

    // Encoder methods
    virtual void setCallsign(const std::string& call);
    virtual void setSsid(uint8_t ssid);
    virtual void setLatitude(float latitude);
    virtual void setNs(uint8_t ns);
    virtual void setLongitude(float longitude);
    virtual void setWe(uint8_t we);
    virtual void setComment(const std::string& comment);
    virtual void setSymbol(uint8_t symbol);
    virtual void setPathType(uint8_t pathType);
    virtual void setBeaconBootup(bool bootup);
    virtual void setWxTransmitPeriod(uint8_t period);
    virtual void setBeaconTransmitPeriod(uint8_t period);
    virtual void setWxDoubleTransmit(bool doubleTransmit);
};

// ============================================================================
// MODE Configuration Decoder/Encoder
// ============================================================================
class ModeConfig : public IModeConfig {
protected:
    const std::vector<uint8_t>& configData;

    template<typename T>
    T readValue(size_t offset) const {
        if (offset + sizeof(T) > configData.size()) {
            throw std::out_of_range("Config data access out of bounds");
        }
        T value;
        std::memcpy(&value, &configData[offset], sizeof(T));
        return value;
    }

    template<typename T>
    void writeValue(size_t offset, T value) {
        if (offset + sizeof(T) > configData.size()) {
            throw std::out_of_range("Config data access out of bounds");
        }
        std::memcpy(&const_cast<std::vector<uint8_t>&>(configData)[offset], &value, sizeof(T));
    }

public:
    ModeConfig(const std::vector<uint8_t>& data);
    virtual ~ModeConfig() = default;

    virtual uint8_t getDigi() const;
    virtual uint8_t getWx() const;
    virtual bool getWxUmb() const;
    virtual bool getWxModbus() const;
    virtual bool getWxDavis() const;
    virtual bool getWxMs5611OrBme() const;
    virtual uint8_t getWxAnemometerConst() const;
    virtual uint8_t getWxDustSensor() const;
    virtual uint8_t getWxPtSensor() const;
    virtual bool getVictron() const;
    virtual bool getDigiViscous() const;
    virtual bool getDigiOnlySsid() const;
    virtual uint8_t getDigiViscousDelay() const;
    virtual uint8_t getDigiDelay100msec() const;
    virtual uint8_t getPowersave() const;
    virtual bool getPowersaveKeepGsm() const;
    virtual bool getGsm() const;

    virtual void setDigi(uint8_t digi);
    virtual void setWx(uint8_t wx);
    virtual void setWxUmb(bool wxUmb);
    virtual void setWxModbus(bool wxModbus);
    virtual void setWxDavis(bool wxDavis);
    virtual void setWxMs5611OrBme(bool sensor);
    virtual void setWxAnemometerConst(uint8_t anemometer);
    virtual void setWxDustSensor(uint8_t dust);
    virtual void setWxPtSensor(uint8_t ptSensor);
    virtual void setVictron(bool victron);
    virtual void setDigiViscous(bool viscous);
    virtual void setDigiOnlySsid(bool onlySsid);
    virtual void setDigiViscousDelay(uint8_t delay);
    virtual void setDigiDelay100msec(uint8_t delay);
    virtual void setPowersave(uint8_t powersave);
    virtual void setPowersaveKeepGsm(bool keepGsm);
    virtual void setGsm(bool gsm);
};

// ============================================================================
// SOURCE Configuration Decoder/Encoder
// ============================================================================
class SourceConfig : public ISourceConfig {
protected:
    const std::vector<uint8_t>& configData;

    template<typename T>
    T readValue(size_t offset) const {
        if (offset + sizeof(T) > configData.size()) {
            throw std::out_of_range("Config data access out of bounds");
        }
        T value;
        std::memcpy(&value, &configData[offset], sizeof(T));
        return value;
    }

    template<typename T>
    void writeValue(size_t offset, T value) {
        if (offset + sizeof(T) > configData.size()) {
            throw std::out_of_range("Config data access out of bounds");
        }
        std::memcpy(&const_cast<std::vector<uint8_t>&>(configData)[offset], &value, sizeof(T));
    }

public:
    SourceConfig(const std::vector<uint8_t>& data);
    virtual ~SourceConfig() = default;

    virtual uint8_t getTemperature() const;
    virtual uint8_t getPressure() const;
    virtual uint8_t getHumidity() const;
    virtual uint8_t getWind() const;

    virtual void setTemperature(uint8_t temp);
    virtual void setPressure(uint8_t pressure);
    virtual void setHumidity(uint8_t humidity);
    virtual void setWind(uint8_t wind);
};

// ============================================================================
// UMB Configuration Decoder/Encoder
// ============================================================================
class UmbConfig : public IUmbConfig {
protected:
    const std::vector<uint8_t>& configData;

    template<typename T>
    T readValue(size_t offset) const {
        if (offset + sizeof(T) > configData.size()) {
            throw std::out_of_range("Config data access out of bounds");
        }
        T value;
        std::memcpy(&value, &configData[offset], sizeof(T));
        return value;
    }

    template<typename T>
    void writeValue(size_t offset, T value) {
        if (offset + sizeof(T) > configData.size()) {
            throw std::out_of_range("Config data access out of bounds");
        }
        std::memcpy(&const_cast<std::vector<uint8_t>&>(configData)[offset], &value, sizeof(T));
    }

public:
    UmbConfig(const std::vector<uint8_t>& data);
    virtual ~UmbConfig() = default;

    virtual uint16_t getSlaveClass() const;
    virtual uint16_t getSlaveId() const;
    virtual uint16_t getChannelWindSpeed() const;
    virtual uint16_t getChannelWindGust() const;
    virtual uint16_t getChannelWindDirection() const;
    virtual uint16_t getChannelTemperature() const;
    virtual uint16_t getChannelQnh() const;

    virtual void setSlaveClass(uint16_t slaveClass);
    virtual void setSlaveId(uint16_t slaveId);
    virtual void setChannelWindSpeed(uint16_t channel);
    virtual void setChannelWindGust(uint16_t channel);
    virtual void setChannelWindDirection(uint16_t channel);
    virtual void setChannelTemperature(uint16_t channel);
    virtual void setChannelQnh(uint16_t channel);
};

// ============================================================================
// RTU Configuration Decoder/Encoder
// ============================================================================
class RtuConfig : public IRtuConfig {
protected:
    const std::vector<uint8_t>& configData;

    template<typename T>
    T readValue(size_t offset) const {
        if (offset + sizeof(T) > configData.size()) {
            throw std::out_of_range("Config data access out of bounds");
        }
        T value;
        std::memcpy(&value, &configData[offset], sizeof(T));
        return value;
    }

    template<typename T>
    void writeValue(size_t offset, T value) {
        if (offset + sizeof(T) > configData.size()) {
            throw std::out_of_range("Config data access out of bounds");
        }
        std::memcpy(&const_cast<std::vector<uint8_t>&>(configData)[offset], &value, sizeof(T));
    }

public:
    RtuConfig(const std::vector<uint8_t>& data);
    virtual ~RtuConfig() = default;

    virtual uint16_t getSlaveSpeed() const;
    virtual uint8_t getSlaveParity() const;
    virtual uint8_t getSlaveStopBits() const;
    virtual uint8_t getUseFullWindData() const;
    virtual uint8_t getTemperatureSrc() const;
    virtual uint8_t getHumiditySrc() const;
    virtual uint8_t getPressureSrc() const;
    virtual uint8_t getWindDir() const;
    virtual uint8_t getWindSpeed() const;
    virtual uint8_t getWindGusts() const;

    virtual void setSlaveSpeed(uint16_t speed);
    virtual void setSlaveParity(uint8_t parity);
    virtual void setSlaveStopBits(uint8_t stopBits);
    virtual void setUseFullWindData(uint8_t useFullWind);
    virtual void setTemperatureSrc(uint8_t tempSrc);
    virtual void setHumiditySrc(uint8_t humiditySrc);
    virtual void setPressureSrc(uint8_t pressureSrc);
    virtual void setWindDir(uint8_t windDir);
    virtual void setWindSpeed(uint8_t windSpeed);
    virtual void setWindGusts(uint8_t windGusts);
};

// ============================================================================
// GSM Configuration Decoder/Encoder
// ============================================================================
class GsmConfig : public IGsmConfig {
protected:
    const std::vector<uint8_t>& configData;

    std::string readString(size_t offset, size_t maxLength) const;
    void writeString(size_t offset, size_t maxLength, const std::string& value);

    template<typename T>
    T readValue(size_t offset) const {
        if (offset + sizeof(T) > configData.size()) {
            throw std::out_of_range("Config data access out of bounds");
        }
        T value;
        std::memcpy(&value, &configData[offset], sizeof(T));
        return value;
    }

    template<typename T>
    void writeValue(size_t offset, T value) {
        if (offset + sizeof(T) > configData.size()) {
            throw std::out_of_range("Config data access out of bounds");
        }
        std::memcpy(&const_cast<std::vector<uint8_t>&>(configData)[offset], &value, sizeof(T));
    }

public:
    GsmConfig(const std::vector<uint8_t>& data);
    virtual ~GsmConfig() = default;

    virtual void getPin(std::string& pin) const;
    virtual std::string getPin() const;
    virtual void getApn(std::string& apn) const;
    virtual std::string getApn() const;
    virtual void getUsername(std::string& username) const;
    virtual std::string getUsername() const;
    virtual void getPassword(std::string& password) const;
    virtual std::string getPassword() const;
    virtual uint8_t getApiEnable() const;
    virtual void getApiBaseUrl(std::string& url) const;
    virtual std::string getApiBaseUrl() const;
    virtual void getApiStationName(std::string& stationName) const;
    virtual std::string getApiStationName() const;
    virtual uint8_t getAprsisEnable() const;
    virtual void getAprsisServer(std::string& server) const;
    virtual std::string getAprsisServer() const;
    virtual uint16_t getAprsisPort() const;
    virtual void getAprsisPpasscode(std::string& passcode) const;
    virtual std::string getAprsisPpasscode() const;

    virtual void setPin(const std::string& pin);
    virtual void setApn(const std::string& apn);
    virtual void setUsername(const std::string& username);
    virtual void setPassword(const std::string& password);
    virtual void setApiEnable(uint8_t apiEnable);
    virtual void setApiBaseUrl(const std::string& url);
    virtual void setApiStationName(const std::string& stationName);
    virtual void setAprsisEnable(uint8_t aprsisEnable);
    virtual void setAprsisServer(const std::string& server);
    virtual void setAprsisPort(uint16_t port);
    virtual void setAprsisPpasscode(const std::string& passcode);
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
    ConfigurationManager(const std::vector<uint8_t>& data)
        : configData(data),
          basicConfig(configData),
          modeConfig(configData),
          sourceConfig(configData),
          umbConfig(configData),
          rtuConfig(configData),
          gsmConfig(configData) {}

    virtual IBasicConfig& getBasicConfig() { return basicConfig; }
    virtual IModeConfig& getModeConfig() { return modeConfig; }
    virtual ISourceConfig& getSourceConfig() { return sourceConfig; }
    virtual IUmbConfig& getUmbConfig() { return umbConfig; }
    virtual IRtuConfig& getRtuConfig() { return rtuConfig; }
    virtual IGsmConfig& getGsmConfig() { return gsmConfig; }

    const std::vector<uint8_t>& getConfigData() const { return configData; }

    virtual void print(PrintVerbosity verbosity);
};
#endif /* SHARED_CONFIG_CONFIGVER0_H_ */
