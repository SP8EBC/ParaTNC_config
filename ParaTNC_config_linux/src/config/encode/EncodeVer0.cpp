/*
 * EncodeVer0.cpp
 *
 *  Created on: Apr 23, 2023
 *      Author: mateusz
 */

#include "EncodeVer0.h"

#include "../Ver0Map.h"

EncodeVer0::EncodeVer0() {

	this->data.reserve(CONFIG__END__OFFSET - 1);

	// prefill vector with dummy data
	this->data.insert(this->data.begin(), CONFIG__END__OFFSET, 0xAA);
}

EncodeVer0::~EncodeVer0() {
}

void EncodeVer0::setEorW(bool _eOrW) {
}

void EncodeVer0::setRtuSlaveSpeed(uint16_t _slaveSpeedBps) {
}

void EncodeVer0::setGsmAprsisEnable(bool _apsisEnable) {
}

void EncodeVer0::setCallsign(std::string &call) {
}

void EncodeVer0::setUmbChannelWinddirection(uint16_t _channelDirection) {
}

void EncodeVer0::setAnemometerPulsesConstant(uint8_t constant) {
}

void EncodeVer0::setPowersave(Powersave _powersave) {
}

void EncodeVer0::setGsmApnUsername(std::string &username) {
}

void EncodeVer0::setRtuConfiguredSourceQnh(uint8_t _rtuSrcQnhPressure) {
}

void EncodeVer0::setRtuSourceConfiguration(Rtu &config, uint8_t sourceId) {
}

void EncodeVer0::setWxDoubleTransmit(bool _doubleTransmit) {
}

void EncodeVer0::setRtuSlaveParity(uint8_t _slaveParity) {
}

void EncodeVer0::setWindSrc(WeatherSource _windSource) {
}

void EncodeVer0::setPath(AprsPath _path) {
}

void EncodeVer0::setUmbChannelWindspeed(uint16_t _channelWindspeed) {
}

void EncodeVer0::setDigiRadiusFilter(uint8_t _radius) {
}

void EncodeVer0::setMs5611orBmeSensor(bool _sensor) {
}

void EncodeVer0::setBeaconAtStartup(bool _beaconAtStartup) {
}

void EncodeVer0::setWxEnabled(bool _enabled) {
}

void EncodeVer0::setGsmEnabled(bool _enabled) {
}

void EncodeVer0::setWxUmbEnabled(bool _enabled) {
}

void EncodeVer0::setVictronEnabled(bool _enabled) {
}

void EncodeVer0::setButtonOneFunction(ButtonFunction _buttonOne) {
}

void EncodeVer0::setUmbSlaveClass(uint16_t _umbSlaveClass) {
}

void EncodeVer0::setKeepGsmAlwaysOn(bool _alwaysOn) {
}

void EncodeVer0::setDigiFilter(DigiFilter _filter) {
}

void EncodeVer0::setRtuSlaveStopBits(uint8_t _slaveStopBits) {
}

void EncodeVer0::setRtuFullWindData(bool _fullWindData) {
}

void EncodeVer0::setDigiEnabled(Digi _digi) {
}

void EncodeVer0::setWxDavisEnabled(bool _enabled) {
}

void EncodeVer0::setUmbChannelWinggusts(uint16_t _channelWindgst) {
}

void EncodeVer0::setUmbChannelTemperature(uint16_t _channelTemperature) {
}

void EncodeVer0::setTemperatureSrc(WeatherSource _temperatureSource) {
}

void EncodeVer0::setLatitude(float _latitude) {
}

void EncodeVer0::setWxModbusEnabled(bool _enabled) {
}

void EncodeVer0::setGsmApnName(std::string &apn) {
}

void EncodeVer0::setRtuConfiguredSourceWindgusts(uint8_t _rtuSrcWidgusts) {
}

void EncodeVer0::setVisvousDelayInSec(uint8_t _seconds) {
}

void EncodeVer0::setDigiAlwaysMessageAndStatus(bool _alwaysMessageAndSts) {
}

void EncodeVer0::setGsmPin(uint32_t _pin) {
}

void EncodeVer0::setPressureSrc(WeatherSource _pressureSource) {
}

void EncodeVer0::setDescritpion(std::string &description) {
}

void EncodeVer0::setGsmApnPassword(std::string &password) {
}

void EncodeVer0::setDigiFilterPrefixList(uint8_t entry, std::string &_value) {
}

void EncodeVer0::setGsmApiEnable(bool _apiEnable) {
}

void EncodeVer0::setRtuConfiguredSourceTemperature(uint8_t _rtuSrcTemperature) {
}

void EncodeVer0::setNorS(bool _nOrS) {
}

void EncodeVer0::setUmbSlaveId(uint16_t _slaveId) {
}

void EncodeVer0::setGsmApiBaseUrl(std::string &password) {
}

void EncodeVer0::setLongitude(float _longitude) {
}

void EncodeVer0::setWxTransmitPeriod(uint8_t wxTransmitPeriod) {
}

void EncodeVer0::setSymbol(AprsSymbol _symbol) {
}

void EncodeVer0::setRtuConfiguredSourceWinddirection(
		uint8_t __rtuSrcWinddirection) {
}

void EncodeVer0::setRtuConfiguredSourceWindspeed(uint8_t _rtuSrcWindspeed) {
}

void EncodeVer0::setButtonTwoFunction(ButtonFunction _buttonTwo) {
}

void EncodeVer0::setBeaconTransmitPeriod(uint8_t beaconTransmitPeriod) {
}

void EncodeVer0::setHumiditySrc(WeatherSource _humiditySource) {
}

void EncodeVer0::setSsid(uint8_t _ssid) {
}

void EncodeVer0::setDigiDelayInMsec(uint8_t _delayInMsec) {
}

bool EncodeVer0::decodeToFile(std::string _fn) {
}

void EncodeVer0::setUmbChannelQnh(uint16_t _channelQnhPressure) {
}
