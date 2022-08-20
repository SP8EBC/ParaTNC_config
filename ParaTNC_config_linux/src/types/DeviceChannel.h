/*
 * DeviceChannel.h
 *
 *  Created on: 13.09.2017
 *      Author: mateusz
 */

#ifndef CONFIG_DEVICECHANNEL_H_
#define CONFIG_DEVICECHANNEL_H_

#include <string>

#include "ChannelValueFoundation.h"
#include "ChannelUsage.h"
#include "ChannelType.h"

using namespace std;

class DeviceChannel {
private:
	ChannelType channelType;
	ChannelUsage channelUsage;
	MeasurementUnit measurementUnit;
	unsigned channelNumber;

	string channelName;

	ChannelValueFoundation* value;

public:
	DeviceChannel();
	virtual ~DeviceChannel();
	static ChannelType CastStringToEnum(string in);
	static ChannelUsage CastUsageStringToEnum(string in);
	static MeasurementUnit CastUnitStringToEnum(string in);

	unsigned getChannelNumber() const;
	void setChannelNumber(unsigned channelNumber);

	ChannelType getChannelType() const;
	void setChannelType(ChannelType channelType);

	const string& getChannelName() const;
	void setChannelName(const string& channelName);

	ChannelUsage getChannelUsage() const;
	void setChannelUsage(ChannelUsage channelUsage);
	MeasurementUnit getMeasurementUnit() const;
	void setMeasurementUnit(MeasurementUnit measurementUnit);
};

#endif /* CONFIG_DEVICECHANNEL_H_ */
