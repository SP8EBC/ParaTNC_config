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
#include "../../types/CardinalDirection.h"

#include "../DefaultConfig.h"

class IConfigEcode {



public:
	virtual ~IConfigEcode() {
		;
	}

	virtual bool decodeToFile(std::string _fn) = 0;

	virtual uint32_t incrementProgrammingCounter() = 0;

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
	virtual void setButtonOneFunction(ButtonFunction _buttonOne) = 0;	// reserved
	virtual void setButtonTwoFunction(ButtonFunction _buttonTwo) = 0;	// reserved


	virtual void setCallsign(const std::string & call) = 0;
	virtual void setSsid(uint8_t _ssid) = 0;
	virtual void setLatitude(float _latitude) = 0;
	virtual void setNorS(CardinalDirection _nOrS) = 0;
	virtual void setLongitude(float _longitude) = 0;
	virtual void setEorW(CardinalDirection _eOrW) = 0;
	virtual void setDescritpion(const std::string & description) = 0;
	virtual void setSymbol(AprsSymbol _symbol) = 0;
	virtual void setPath(const AprsPath _path) = 0;
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
	virtual void setRtuSourceConfiguration(const Rtu & config, uint8_t sourceId) = 0;

	virtual void setGsmPin(uint32_t _pin) = 0;
	virtual void setGsmApnName(const std::string & apn) = 0;
	virtual void setGsmApnUsername(const std::string & username) = 0;
	virtual void setGsmApnPassword(const std::string & password) = 0;
	virtual void setGsmApiEnable(bool _apiEnable) = 0;
	virtual void setGsmApiBaseUrl(const std::string & password) = 0;
	virtual void setGsmAprsisEnable(bool _apsisEnable) = 0;
	virtual void setGsmAprsisServer(const std::string & server ) = 0;
	virtual void setGsmAprsisServerPort (uint16_t port) = 0;
	virtual void setGsmAprsisPasscode	(uint32_t passcode) = 0;

	virtual void loadDefaults() 	{
		setDigiEnabled				(DEFAULT_CONFIG_DIGI_ENABLE);
		setVisvousDelayInSec		(DEFAULT_CONFIG_VISCOUS_DELAY);
		setDigiDelayInMsec			(DEFAULT_CONFIG_DIGI_DELAY);
		setWxEnabled				(DEFAULT_CONFIG_WX_ENABLED);
		setWxUmbEnabled				(DEFAULT_CONFIG_WX_UMB_ENABLED);
		setWxModbusEnabled			(DEFAULT_CONFIG_WX_MODBUS_ENABLED);
		setWxDavisEnabled			(DEFAULT_CONFIG_WX_DAVIS_ENABLED);
		setMs5611orBmeSensor		(DEFAULT_CONFIG_MS5611_OR_BME_SENSOR);
		setAnemometerPulsesConstant	(DEFAULT_CONFIG_ANEMOMETER_PULSES);
		setVictronEnabled			(DEFAULT_CONFIG_VICTRON_ENABLED);
		setPowersave				(DEFAULT_CONFIG_POWERSAVE);
		setGsmEnabled				(DEFAULT_CONFIG_GSM_ENABLED);
		setKeepGsmAlwaysOn			(DEFAULT_CONFIG_KEEP_GSM_ALWAYS_ON);

		setCallsign			(DEFAULT_CONFIG_CALLSIGN);
		setSsid				(DEFAULT_CONFIG_SSID);
		setLatitude			(DEFAULT_CONFIG_LATITUDE);
		setNorS				(DEFAULT_CONFIG_N_OR_S);
		setLongitude		(DEFAULT_CONFIG_LONGITUDE);
		setEorW				(DEFAULT_CONFIG_E_OR_W);
		setDescritpion		(DEFAULT_CONFIG_DESCRIPTION);
		setSymbol			(DEFAULT_CONFIG_SYMBOL);
		setPath				(DEFAULT_CONFIG_PATH);
		setBeaconAtStartup	(DEFAULT_CONFIG_BEACON_START);
		setWxTransmitPeriod	(DEFAULT_CONFIG_WX_PERIOD);
		setBeaconTransmitPeriod(DEFAULT_CONFIG_BEACON_PERIO);
		setWxDoubleTransmit	(DEFAULT_CONFIG_WX_DOUBLE_TX);

		setTemperatureSrc		(DEFAULT_CONFIG_SOURCE_TEMPERATURE);
		setPressureSrc			(DEFAULT_CONFIG_SOURCE_PRESSURE);
		setWindSrc				(DEFAULT_CONFIG_SOURCE_WIND);
		setHumiditySrc			(DEFAULT_CONFIG_SOURCE_HUMIDITY);

		setUmbSlaveClass				(DEFAULT_CONFIG_UMB_SLAVE_CLASS);
		setUmbSlaveId					(DEFAULT_CONFIG_UMB_SLAVE_ID);
		setUmbChannelWindspeed			(DEFAULT_CONFIG_UMB_CHANNEL_WINDSPEED);
		setUmbChannelWinggusts			(DEFAULT_CONFIG_UMB_CHANNEL_WINDGUSTS);
		setUmbChannelWinddirection		(DEFAULT_CONFIG_UMB_CHANNEL_WINDDIR);
		setUmbChannelTemperature		(DEFAULT_CONDIG_UMB_CHANNEL_TEMPERATURE);
		setUmbChannelQnh				(DEFAULT_CONFIG_UMB_CHANNEL_QNH);

		setRtuSlaveSpeed					(DEFAULT_CONFIG_RTU_SLAVE_SPEED);
		setRtuSlaveParity					(DEFAULT_CONFIG_RTU_SLAVE_PARITY);
		setRtuSlaveStopBits					(DEFAULT_CONFIG_RTU_STOP_BITS);
		setRtuFullWindData					(DEFAULT_CONFIG_RTU_FULL_WIND);
		setRtuConfiguredSourceWindspeed		(DEFAULT_CONFIG_RTU_SOURCE_WINDSPEED);
		setRtuConfiguredSourceWindgusts		(DEFAULT_CONFIG_RTU_SOURCE_WINDGUST);
		setRtuConfiguredSourceWinddirection	(DEFAULT_CONFIG_RTU_SOURCE_WINDDIR);
		setRtuConfiguredSourceTemperature	(DEFAULT_CONFIG_RTU_SOURCE_TEMP);
		setRtuConfiguredSourceQnh			(DEFAULT_CONFIG_RTU_SOURCE_PRESSURE);

		setGsmPin			(DEFAULT_CONFIG_GSM_PIN);
		setGsmApnName		(DEFAULT_CONFIG_GSM_APNNAME);
		setGsmApnUsername	(DEFAULT_CONFIG_GSM_USERNAME);
		setGsmApnPassword	(DEFAULT_CONFIG_GSM_PASSWORD);
		setGsmApiEnable		(DEFAULT_CONFIG_GSM_API_ENABLE);
		setGsmApiBaseUrl	(DEFAULT_CONFIG_GSM_API_BASEURL);
		setGsmAprsisEnable	(DEFAULT_CONFIG_APRSIS_ENABLE);
		setGsmAprsisServer	(DEFAULT_CONFIG_APRSIS_SERVER);
		setGsmAprsisServerPort(DEFAULT_CONFIG_APRSIS_PORT);
		setGsmAprsisPasscode(DEFAULT_CONFIG_APRSIS_PASSCODE);
	}

};

#endif /* SRC_CONFIG_ENCODE_ICONFIGENCODE_H_ */
