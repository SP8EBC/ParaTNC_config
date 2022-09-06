/*
 * DecodeVer0.h
 *
 *  Created on: Sep 3, 2022
 *      Author: mateusz
 */

#ifndef SRC_CONFIG_DECODE_DECODEVER0_H_
#define SRC_CONFIG_DECODE_DECODEVER0_H_

#include "IConfigDecode.h"

class DecodeVer0: public IConfigDecode {

	const std::vector<uint8_t> data;

public:
	DecodeVer0(const std::vector<uint8_t> & configData);

	virtual ~DecodeVer0();
	virtual bool getMs5611orBmeSensor() override;
	virtual DigiFilter getDigiFilter() override;
	virtual uint8_t getWxTransmitPeriod() override;
	virtual ButtonFunction getButtonTwoFunction() override;
	virtual bool getKeepGsmAlwaysOn() override;
	virtual bool getGsmApiEnable() override;
	virtual uint8_t getAnemometerPulsesConstant() override;
	virtual uint8_t getRtuSlaveParity() override;
	virtual uint8_t getSsid() override;
	virtual void getGsmApiBaseUrl(std::string &password) override;
	virtual Digi getDigiEnabled() override;
	virtual Powersave getPowersave() override;
	virtual AprsPath getPath() override;
	virtual uint8_t getRtuChannelWindgusts() override;
	virtual bool getGsmAprsisEnable() override;
	virtual bool getNorS() override;
	virtual std::string getDigiFilterPrefixList(uint8_t entry) override;
	virtual uint16_t getUmbChannelWinddirection() override;
	virtual uint8_t getRtuChannelWinddirection() override;
	virtual uint8_t getBeaconTransmitPeriod() override;
	virtual uint8_t getDigiDelayInMsec() override;
	virtual uint32_t getGsmPin() override;
	virtual uint16_t getUmbChannelWindspeed() override;
	virtual uint16_t getUmbChannelQnh() override;
	virtual uint16_t getRtuSlaveSpeed() override;
	virtual void getGsmApnName(std::string &apn) override;
	virtual void getCallsign(std::string &call) override;
	virtual uint16_t getUmbSlaveId() override;
	virtual uint8_t getDigiRadiusFilter() override;
	virtual bool getEorW() override;
	virtual WeatherSource getWindSrc() override;
	virtual AprsSymbol getSymbol() override;
	virtual bool getVictronEnabled() override;
	virtual bool getGsmEnabled() override;
	virtual uint8_t getVisvousDelayInSec() override;
	virtual bool getBeaconAtStartup() override;
	virtual Rtu getRtuChannelConfig(uint8_t channel) override;
	virtual uint8_t getRtuChannelTemperature() override;
	virtual void getGsmApnUsername(std::string &username) override;
	virtual ButtonFunction getButtonOneFunction() override;
	virtual uint16_t getUmbSlaveClass() override;
	virtual void getDescritpion(std::string &description) override;
	virtual uint16_t getUmbChannelTemperature() override;
	virtual uint16_t getUmbChannelWinggusts() override;
	virtual WeatherSource getPressureSrc() override;
	virtual bool getWxModbusEnabled() override;
	virtual float getLongitude() override;
	virtual float getLatitude() override;
	virtual WeatherSource getTemperatureSrc() override;
	virtual bool getRtuFullWindData() override;
	virtual uint8_t getRtuSlaveStopBits() override;
	virtual uint8_t getRtuChannelWindspeed() override;
	virtual bool getDigiAlwaysMessageAndStatus() override;
	virtual bool getWxUmbEnabled() override;
	virtual bool getWxEnabled() override;
	virtual bool getWxDavisEnabled() override;
	virtual uint8_t getRtuChannelQnh() override;
	virtual bool getWxDoubleTransmit() override;
	virtual WeatherSource getHumiditySrc() override;
	virtual void getGsmApnPassword(std::string &password) override;
};

#endif /* SRC_CONFIG_DECODE_DECODEVER0_H_ */
