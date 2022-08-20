/*
 * DeviceChannel.cpp
 *
 *  Created on: 13.09.2017
 *      Author: mateusz
 */

#include "DeviceChannel.h"
#include "../exceptions/UsageParseError.h"


DeviceChannel::DeviceChannel() {
	// TODO Auto-generated constructor stub

	this->channelNumber = 0;
	this->channelType = NL;
	this->channelUsage = NONE;
	this->measurementUnit = UNINITIALIZED;
}

unsigned DeviceChannel::getChannelNumber() const {
	return channelNumber;
}

void DeviceChannel::setChannelNumber(unsigned channelNumber) {
	this->channelNumber = channelNumber;
}

DeviceChannel::~DeviceChannel() {
	// TODO Auto-generated destructor stub
}

ChannelType DeviceChannel::getChannelType() const {
	return channelType;
}

const string& DeviceChannel::getChannelName() const {
	return channelName;
}

ChannelType DeviceChannel::CastStringToEnum(string in) {
	ChannelType out = NL;

	if (in == "UNSIGNED_CHAR")
		out = UNSIGNED_CHAR;
	else if (in == "SIGNED_CHAR")
		out = SIGNED_CHAR;
	else if (in == "UNSIGNED_SHORT")
		out = UNSIGNED_SHORT;
	else if (in == "SIGNED_SHORT")
		out = SIGNED_SHORT;
	else if (in == "UNSIGNED_INT")
		out = UNSIGNED_INT;
	else if (in == "SIGNED_INT")
		out = SIGNED_INT;
	else if (in == "FLOAT")
		out = FLOAT;
	else if (in == "DOUBLE")
		out = DOUBLE;
	else;

	return out;

}

ChannelUsage DeviceChannel::CastUsageStringToEnum(string in) {
	ChannelUsage out = NONE;

	if (in == "WINDSPEED")
		out = WINDSPEED;
	else if (in == "WINDGUSTS")
		out = WINDGUSTS;
	else if (in == "WINDDIR")
		out = WINDDIR;
	else if (in == "TEMPERATURE")
		out = TEMPERATURE;
	else if (in == "PRESSURE")
		out = PRESSURE;
	else if (in == "HUMIDITY")
		out = HUMIDITY;
	else
		throw UsageParseError();

	return out;

}

ChannelUsage DeviceChannel::getChannelUsage() const {
	return channelUsage;
}

MeasurementUnit DeviceChannel::CastUnitStringToEnum(string in) {
	MeasurementUnit out = UNINITIALIZED;

	if (in == "m/s")
		out = MS;
	else if (in == "km/h")
		out = KMH;
	else if (in == "kts")
		out = KTS;
	else if (in == "hPa")
		out = HPA;
	else if (in == "degC")
		out = DEGC;
	else if (in == "degF")
		out = DEGF;
	else if (in == "percent")
		out = PERCENT;
	else
		throw UsageParseError();

	return out;

}

MeasurementUnit DeviceChannel::getMeasurementUnit() const {
	return measurementUnit;
}

void DeviceChannel::setMeasurementUnit(MeasurementUnit measurementUnit) {
	this->measurementUnit = measurementUnit;
}

void DeviceChannel::setChannelUsage(ChannelUsage channelUsage) {
	this->channelUsage = channelUsage;
}

void DeviceChannel::setChannelName(const string& channelName) {
	this->channelName = channelName;
}

void DeviceChannel::setChannelType(ChannelType channelType) {
	this->channelType = channelType;
}
