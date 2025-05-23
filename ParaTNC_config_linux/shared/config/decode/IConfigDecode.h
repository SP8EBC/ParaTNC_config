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
#include "stdint.h"

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

	virtual bool decodeToFile(std::string _fn) = 0;

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

	virtual uint8_t getWxDustSensor() = 0;	// from EA16

	virtual uint16_t getBatteryScalingA() = 0;		// from EA20
	virtual uint16_t getBatteryScalingB() = 0;		// from EA20

	virtual ButtonFunction getButtonOneFunction() = 0;			// from EA22
	virtual ButtonFunction getButtonTwoFunction() = 0;			// from EA22

	virtual bool getPowersaveKeepGsmEnabled() = 0;		// from EA07

	virtual uint32_t getUdsSecurityAccess() = 0;	// from EC00

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
	virtual uint8_t getRtuConfiguredSourceWindspeed() = 0;
	virtual uint8_t getRtuConfiguredSourceWindgusts() = 0;
	virtual uint8_t getRtuConfiguredSourceWinddirection() = 0;
	virtual uint8_t getRtuConfiguredSourceTemperature() = 0;
	virtual uint8_t getRtuConfiguredSourceQnh() = 0;
	virtual Rtu getRtuSourceConfig(uint8_t sourceIdx) = 0;

	virtual uint32_t getGsmPin() = 0;
	virtual void getGsmApnName(std::string & apn) = 0;
	virtual void getGsmApnUsername(std::string & username) = 0;
	virtual void getGsmApnPassword(std::string & password) = 0;
	virtual bool getGsmApiEnable() = 0;
	virtual void getGsmApiBaseUrl(std::string & password) = 0;
	virtual void getGsmApiStationName(std::string & _station_name) = 0;
	virtual bool getGsmAprsisEnable() = 0;

};

#endif /* SRC_CONFIG_DECODE_ICONFIGDECODE_H_ */
