/*
 * DecodeVer0.h
 *
 *  Created on: Sep 3, 2022
 *      Author: mateusz
 */

#ifndef SRC_CONFIG_DECODE_DECODEVER0_H_
#define SRC_CONFIG_DECODE_DECODEVER0_H_

#include "IConfigDecode.h"

/**
 * Decode configuration data for versions EA16 to EA20
 */
class DecodeVer0: public IConfigDecode {

	const std::vector<uint8_t> data;

public:
	DecodeVer0(const std::vector<uint8_t> & configData);
	virtual ~DecodeVer0();

	virtual bool decodeToFile(std::string _fn);

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
	virtual void getGsmApiStationName(std::string & _station_name);
	virtual Digi getDigiEnabled()  ;
	virtual Powersave getPowersave()  ;
	virtual AprsPath getPath()  ;
	virtual uint8_t getRtuConfiguredSourceWindgusts()  ;
	virtual bool getGsmAprsisEnable()  ;
	virtual bool getNorS()  ;
	virtual std::string getDigiFilterPrefixList(uint8_t entry)  ;
	virtual uint16_t getUmbChannelWinddirection()  ;
	virtual uint8_t getRtuConfiguredSourceWinddirection()  ;
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
	virtual Rtu getRtuSourceConfig(uint8_t channelConfig)  ;
	virtual uint8_t getRtuConfiguredSourceTemperature()  ;
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
	virtual uint8_t getRtuConfiguredSourceWindspeed()  ;
	virtual bool getDigiAlwaysMessageAndStatus()  ;
	virtual bool getWxUmbEnabled()  ;
	virtual bool getWxEnabled()  ;
	virtual bool getWxDavisEnabled()  ;
	virtual uint8_t getRtuConfiguredSourceQnh()  ;
	virtual bool getWxDoubleTransmit()  ;
	virtual WeatherSource getHumiditySrc()  ;
	virtual void getGsmApnPassword(std::string &password)  ;

	virtual uint8_t getWxDustSensor();	// from EA16

	virtual uint16_t getBatteryScalingA();		// from EA20
	virtual uint16_t getBatteryScalingB();		// from EA20

	virtual bool getPowersaveKeepGsmEnabled();		// from EA07

	virtual uint32_t getUdsSecurityAccess();	// from EC00
};

#endif /* SRC_CONFIG_DECODE_DECODEVER0_H_ */
