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
	virtual bool getMs5611orBmeSensor()  ;
	virtual DigiFilter getDigiFilter()  ;
	virtual uint8_t getWxTransmitPeriod()  ;
	virtual ButtonFunction getButtonTwoFunction()  ;
	virtual bool getKeepGsmAlwaysOn()  ;
	virtual bool getGsmApiEnable()  ;
	virtual uint8_t getAnemometerPulsesConstant()  ;
	virtual uint8_t getRtuSlaveParity()  ;
	virtual uint8_t getSsid()  ;
	virtual void getGsmApiBaseUrl(std::string &password)  ;
	virtual Digi getDigiEnabled()  ;
	virtual Powersave getPowersave()  ;
	virtual AprsPath getPath()  ;
	virtual uint8_t getRtuChannelWindgusts()  ;
	virtual bool getGsmAprsisEnable()  ;
	virtual bool getNorS()  ;
	virtual std::string getDigiFilterPrefixList(uint8_t entry)  ;
	virtual uint16_t getUmbChannelWinddirection()  ;
	virtual uint8_t getRtuChannelWinddirection()  ;
	virtual uint8_t getBeaconTransmitPeriod()  ;
	virtual uint8_t getDigiDelayInMsec()  ;
	virtual uint32_t getGsmPin()  ;
	virtual uint16_t getUmbChannelWindspeed()  ;
	virtual uint16_t getUmbChannelQnh()  ;
	virtual uint16_t getRtuSlaveSpeed()  ;
	virtual void getGsmApnName(std::string &apn)  ;
	virtual void getCallsign(std::string &call)  ;
	virtual uint16_t getUmbSlaveId()  ;
	virtual uint8_t getDigiRadiusFilter()  ;
	virtual bool getEorW()  ;
	virtual WeatherSource getWindSrc()  ;
	virtual AprsSymbol getSymbol()  ;
	virtual bool getVictronEnabled()  ;
	virtual bool getGsmEnabled()  ;
	virtual uint8_t getVisvousDelayInSec()  ;
	virtual bool getBeaconAtStartup()  ;
	virtual Rtu getRtuChannelConfig(uint8_t channel)  ;
	virtual uint8_t getRtuChannelTemperature()  ;
	virtual void getGsmApnUsername(std::string &username)  ;
	virtual ButtonFunction getButtonOneFunction()  ;
	virtual uint16_t getUmbSlaveClass()  ;
	virtual void getDescritpion(std::string &description)  ;
	virtual uint16_t getUmbChannelTemperature()  ;
	virtual uint16_t getUmbChannelWinggusts()  ;
	virtual WeatherSource getPressureSrc()  ;
	virtual bool getWxModbusEnabled()  ;
	virtual float getLongitude()  ;
	virtual float getLatitude()  ;
	virtual WeatherSource getTemperatureSrc()  ;
	virtual bool getRtuFullWindData()  ;
	virtual uint8_t getRtuSlaveStopBits()  ;
	virtual uint8_t getRtuChannelWindspeed()  ;
	virtual bool getDigiAlwaysMessageAndStatus()  ;
	virtual bool getWxUmbEnabled()  ;
	virtual bool getWxEnabled()  ;
	virtual bool getWxDavisEnabled()  ;
	virtual uint8_t getRtuChannelQnh()  ;
	virtual bool getWxDoubleTransmit()  ;
	virtual WeatherSource getHumiditySrc()  ;
	virtual void getGsmApnPassword(std::string &password)  ;
};

#endif /* SRC_CONFIG_DECODE_DECODEVER0_H_ */
