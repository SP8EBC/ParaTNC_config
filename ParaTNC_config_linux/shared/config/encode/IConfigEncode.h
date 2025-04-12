/*
 * IConfigEncode.h
 *
 *  Created on: Apr 23, 2023
 *      Author: mateusz
 */

#ifndef SRC_CONFIG_ENCODE_ICONFIGENCODE_H_
#define SRC_CONFIG_ENCODE_ICONFIGENCODE_H_

#include <string>
#include <stdint.h>
#include <vector>

#include "../../types/AprsSymbol.h"
#include "../../types/AprsPath.h"
#include "../../types/Powersave.h"
#include "../../types/WeatherSource.h"
#include "../../types/Rtu.h"
#include "../../types/Digi.h"
#include "../../types/DigiFilter.h"
#include "../../types/ButtonFunction.h"

class IConfigEcode {



public:
	virtual ~IConfigEcode() {
		;
	}

	virtual bool decodeToFile(std::string _fn) = 0;

	virtual void setDigiEnabled(Digi _digi) = 0;
	virtual void setVisvousDelayInSec(uint8_t _seconds) = 0;
	virtual void setDigiDelayInMsec(uint8_t _delayInMsec) = 0;
	virtual void setWxEnabled(bool _enabled) = 0;
	virtual void setWxUmbEnabled(bool _enabled) = 0;
	virtual void setWxModbusEnabled(bool _enabled) = 0;
	virtual void setWxDavisEnabled(bool _enabled) = 0;
	virtual void setMs5611orBmeSensor(bool _sensor) = 0;
	virtual void setAnemometerPulsesConstant(uint8_t constant) = 0;
	virtual void setVictronEnabled(bool _enabled) = 0;
	virtual void setPowersave(Powersave _powersave) = 0;
	virtual void setGsmEnabled(bool _enabled) = 0;
	virtual void setKeepGsmAlwaysOn(bool _alwaysOn) = 0;

	virtual void setDigiRadiusFilter(uint8_t _radius) = 0;	// reserved
	virtual void setDigiAlwaysMessageAndStatus(bool _alwaysMessageAndSts) = 0;	// reserved
	virtual void setDigiFilter(DigiFilter _filter) = 0;		// reserved
	virtual void setDigiFilterPrefixList(uint8_t entry, std::string & _value) = 0;		// reserved

	virtual void setWxDustSensor(uint8_t _wx_dust_sensor) = 0;	// from EA16

	virtual void setBatteryScalingA(uint16_t _a) = 0;		// from EA20
	virtual void setBatteryScalingB(uint16_t _b) = 0;		// from EA20

	virtual void setButtonOneFunction(ButtonFunction _buttonOne) = 0;	// from EA22
	virtual void setButtonTwoFunction(ButtonFunction _buttonTwo) = 0;	// from EA22

	virtual void setPowersaveKeepGsmEnabled(bool _enabled) = 0;		// from EA07

	virtual void setUdsSecurityAccess(uint32_t _sec_access) = 0;	// from EC00

	virtual void setCallsign(std::string & call) = 0;
	virtual void setSsid(uint8_t _ssid) = 0;
	virtual void setLatitude(float _latitude) = 0;
	virtual void setNorS(bool _nOrS) = 0;
	virtual void setLongitude(float _longitude) = 0;
	virtual void setEorW(bool _eOrW) = 0;
	virtual void setDescritpion(std::string & description) = 0;
	virtual void setSymbol(AprsSymbol _symbol) = 0;
	virtual void setPath(AprsPath _path) = 0;
	virtual void setBeaconAtStartup(bool _beaconAtStartup) = 0;
	virtual void setWxTransmitPeriod(uint8_t wxTransmitPeriod) = 0;
	virtual void setBeaconTransmitPeriod(uint8_t beaconTransmitPeriod) = 0;
	virtual void setWxDoubleTransmit(bool _doubleTransmit) = 0;

	virtual void setTemperatureSrc(WeatherSource _temperatureSource) = 0;
	virtual void setPressureSrc(WeatherSource _pressureSource) = 0;
	virtual void setWindSrc(WeatherSource _windSource) = 0;
	virtual void setHumiditySrc(WeatherSource _humiditySource) = 0;

	virtual void setUmbSlaveClass(uint16_t _umbSlaveClass) = 0;
	virtual void setUmbSlaveId(uint16_t _slaveId) = 0;
	virtual void setUmbChannelWindspeed(uint16_t _channelWindspeed) = 0;
	virtual void setUmbChannelWinggusts(uint16_t _channelWindgst) = 0;
	virtual void setUmbChannelWinddirection(uint16_t _channelDirection) = 0;
	virtual void setUmbChannelTemperature(uint16_t _channelTemperature) = 0;
	virtual void setUmbChannelQnh(uint16_t _channelQnhPressure) = 0;

	virtual void setRtuSlaveSpeed(uint16_t _slaveSpeedBps) = 0;
	virtual void setRtuSlaveParity(uint8_t _slaveParity) = 0;
	virtual void setRtuSlaveStopBits(uint8_t _slaveStopBits) = 0;
	virtual void setRtuFullWindData(bool _fullWindData) = 0;
	virtual void setRtuConfiguredSourceWindspeed(uint8_t _rtuSrcWindspeed) = 0;
	virtual void setRtuConfiguredSourceWindgusts(uint8_t _rtuSrcWidgusts) = 0;
	virtual void setRtuConfiguredSourceWinddirection(uint8_t __rtuSrcWinddirection) = 0;
	virtual void setRtuConfiguredSourceTemperature(uint8_t _rtuSrcTemperature) = 0;
	virtual void setRtuConfiguredSourceQnh(uint8_t _rtuSrcQnhPressure) = 0;
	virtual void setRtuSourceConfiguration(Rtu & config, uint8_t sourceId) = 0;

	virtual void setGsmPin(uint32_t _pin) = 0;
	virtual void setGsmApnName(std::string & apn) = 0;
	virtual void setGsmApnUsername(std::string & username) = 0;
	virtual void setGsmApnPassword(std::string & password) = 0;
	virtual void setGsmApiEnable(bool _apiEnable) = 0;
	virtual void setGsmApiBaseUrl(std::string & password) = 0;
	virtual void setGsmAprsisEnable(bool _apsisEnable) = 0;

};

#endif /* SRC_CONFIG_ENCODE_ICONFIGENCODE_H_ */
