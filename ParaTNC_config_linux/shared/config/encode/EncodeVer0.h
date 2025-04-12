/*
 * EncodeVer0.h
 *
 *  Created on: Apr 23, 2023
 *      Author: mateusz
 */

#ifndef SRC_CONFIG_ENCODE_ENCODEVER0_H_
#define SRC_CONFIG_ENCODE_ENCODEVER0_H_

#include "IConfigEncode.h"

#include <vector>

/**
 * Encodes configuration data for versions EA16 to EA20
 */
class EncodeVer0: public IConfigEcode {

	/**
	 * Configuration data to send to the controller
	 */
	std::vector<uint8_t> data;

public:
	EncodeVer0();
	virtual ~EncodeVer0();
	virtual void setEorW(bool _eOrW);
	virtual void setRtuSlaveSpeed(uint16_t _slaveSpeedBps);
	virtual void setGsmAprsisEnable(bool _apsisEnable);
	virtual void setCallsign(std::string &call);
	virtual void setUmbChannelWinddirection(uint16_t _channelDirection);
	virtual void setAnemometerPulsesConstant(uint8_t constant);
	virtual void setPowersave(Powersave _powersave);
	virtual void setGsmApnUsername(std::string &username);
	virtual void setRtuConfiguredSourceQnh(uint8_t _rtuSrcQnhPressure);
	virtual void setRtuSourceConfiguration(Rtu &config, uint8_t sourceId);
	virtual void setWxDoubleTransmit(bool _doubleTransmit);
	virtual void setRtuSlaveParity(uint8_t _slaveParity);
	virtual void setWindSrc(WeatherSource _windSource);
	virtual void setPath(AprsPath _path);
	virtual void setUmbChannelWindspeed(uint16_t _channelWindspeed);
	virtual void setDigiRadiusFilter(uint8_t _radius);
	virtual void setMs5611orBmeSensor(bool _sensor);
	virtual void setBeaconAtStartup(bool _beaconAtStartup);
	virtual void setWxEnabled(bool _enabled);
	virtual void setGsmEnabled(bool _enabled);
	virtual void setWxUmbEnabled(bool _enabled);
	virtual void setVictronEnabled(bool _enabled);
	virtual void setButtonOneFunction(ButtonFunction _buttonOne);
	virtual void setUmbSlaveClass(uint16_t _umbSlaveClass);
	virtual void setKeepGsmAlwaysOn(bool _alwaysOn);
	virtual void setDigiFilter(DigiFilter _filter);
	virtual void setRtuSlaveStopBits(uint8_t _slaveStopBits);
	virtual void setRtuFullWindData(bool _fullWindData);
	virtual void setDigiEnabled(Digi _digi);
	virtual void setWxDavisEnabled(bool _enabled);
	virtual void setUmbChannelWinggusts(uint16_t _channelWindgst);
	virtual void setUmbChannelTemperature(uint16_t _channelTemperature);
	virtual void setTemperatureSrc(WeatherSource _temperatureSource);
	virtual void setLatitude(float _latitude);
	virtual void setWxModbusEnabled(bool _enabled);
	virtual void setGsmApnName(std::string &apn);
	virtual void setRtuConfiguredSourceWindgusts(uint8_t _rtuSrcWidgusts);
	virtual void setVisvousDelayInSec(uint8_t _seconds);
	virtual void setDigiAlwaysMessageAndStatus(bool _alwaysMessageAndSts);
	virtual void setGsmPin(uint32_t _pin);
	virtual void setPressureSrc(WeatherSource _pressureSource);
	virtual void setDescritpion(std::string &description);
	virtual void setGsmApnPassword(std::string &password);
	virtual void setDigiFilterPrefixList(uint8_t entry, std::string &_value);
	virtual void setGsmApiEnable(bool _apiEnable);
	virtual void setRtuConfiguredSourceTemperature(uint8_t _rtuSrcTemperature);
	virtual void setNorS(bool _nOrS);
	virtual void setUmbSlaveId(uint16_t _slaveId);
	virtual void setGsmApiBaseUrl(std::string &password);
	virtual void setLongitude(float _longitude);
	virtual void setWxTransmitPeriod(uint8_t wxTransmitPeriod);
	virtual void setSymbol(AprsSymbol _symbol);
	virtual void setRtuConfiguredSourceWinddirection(
			uint8_t __rtuSrcWinddirection);
	virtual void setRtuConfiguredSourceWindspeed(uint8_t _rtuSrcWindspeed);
	virtual void setButtonTwoFunction(ButtonFunction _buttonTwo);
	virtual void setBeaconTransmitPeriod(uint8_t beaconTransmitPeriod);
	virtual void setHumiditySrc(WeatherSource _humiditySource);
	virtual void setSsid(uint8_t _ssid);
	virtual void setDigiDelayInMsec(uint8_t _delayInMsec);
	virtual bool decodeToFile(std::string _fn);
	virtual void setUmbChannelQnh(uint16_t _channelQnhPressure);

	virtual void setBatteryScalingA(uint16_t _a);		// from EA20
	virtual void setBatteryScalingB(uint16_t _b);		// from EA20
};

#endif /* SRC_CONFIG_ENCODE_ENCODEVER0_H_ */
