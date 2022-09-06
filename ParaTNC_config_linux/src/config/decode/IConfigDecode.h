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
#include <cstdint>

#include "../../types/AprsSymbol.h"
#include "../../types/AprsPath.h"
#include "../../types/Powersave.h"
#include "../../types/WeatherSource.h"
#include "../../types/Rtu.h"
#include "../../types/Digi.h"
#include "../../types/DigiFilter.h"
#include "../../types/ButtonFunction.h"


class IConfigDecode {
public:
	virtual ~IConfigDecode() {
		;
	}

	virtual Digi getDigiEnabled() = 0;
	virtual uint8_t getVisvousDelayInSec() = 0;
	virtual uint8_t getDigiDelayInMsec() = 0;
	virtual bool getWxEnabled() = 0;
	virtual bool getWxUmbEnabled() = 0;
	virtual bool getWxModbusEnabled() = 0;
	virtual bool getWxDavisEnabled() = 0;
	virtual bool getMs5611orBmeSensor() = 0;
	virtual uint8_t getAnemometerPulsesConstant() = 0;
	virtual bool getVictronEnabled() = 0;
	virtual Powersave getPowersave() = 0;
	virtual bool getGsmEnabled() = 0;
	virtual bool getKeepGsmAlwaysOn() = 0;

	virtual uint8_t getDigiRadiusFilter() = 0;	// reserved
	virtual bool getDigiAlwaysMessageAndStatus() = 0;	// reserved
	virtual DigiFilter getDigiFilter() = 0;		// reserved
	virtual std::string getDigiFilterPrefixList(uint8_t entry) = 0;		// reserved
	virtual ButtonFunction getButtonOneFunction() = 0;
	virtual ButtonFunction getButtonTwoFunction() = 0;


	virtual void getCallsign(std::string & call) = 0;
	virtual uint8_t getSsid() = 0;
	virtual float getLatitude() = 0;
	virtual bool getNorS() = 0;
	virtual float getLongitude() = 0;
	virtual bool getEorW() = 0;
	virtual void getDescritpion(std::string & description) = 0;
	virtual AprsSymbol getSymbol() = 0;
	virtual AprsPath getPath() = 0;
	virtual bool getBeaconAtStartup() = 0;
	virtual uint8_t getWxTransmitPeriod() = 0;
	virtual uint8_t getBeaconTransmitPeriod() = 0;
	virtual bool getWxDoubleTransmit() = 0;

	virtual WeatherSource getTemperatureSrc() = 0;
	virtual WeatherSource getPressureSrc() = 0;
	virtual WeatherSource getWindSrc() = 0;
	virtual WeatherSource getHumiditySrc() = 0;

	virtual uint16_t getUmbSlaveClass() = 0;
	virtual uint16_t getUmbSlaveId() = 0;
	virtual uint16_t getUmbChannelWindspeed() = 0;
	virtual uint16_t getUmbChannelWinggusts() = 0;
	virtual uint16_t getUmbChannelWinddirection() = 0;
	virtual uint16_t getUmbChannelTemperature() = 0;
	virtual uint16_t getUmbChannelQnh() = 0;

	virtual uint16_t getRtuSlaveSpeed() = 0;
	virtual uint8_t getRtuSlaveParity() = 0;
	virtual uint8_t getRtuSlaveStopBits() = 0;
	virtual bool getRtuFullWindData() = 0;
	virtual uint8_t getRtuChannelWindspeed() = 0;
	virtual uint8_t getRtuChannelWindgusts() = 0;
	virtual uint8_t getRtuChannelWinddirection() = 0;
	virtual uint8_t getRtuChannelTemperature() = 0;
	virtual uint8_t getRtuChannelQnh() = 0;
	virtual Rtu getRtuChannelConfig(uint8_t channel) = 0;

	virtual uint32_t getGsmPin() = 0;
	virtual void getGsmApnName(std::string & apn) = 0;
	virtual void getGsmApnUsername(std::string & username) = 0;
	virtual void getGsmApnPassword(std::string & password) = 0;
	virtual bool getGsmApiEnable() = 0;
	virtual void getGsmApiBaseUrl(std::string & password) = 0;
	virtual bool getGsmAprsisEnable() = 0;

};

#endif /* SRC_CONFIG_DECODE_ICONFIGDECODE_H_ */
