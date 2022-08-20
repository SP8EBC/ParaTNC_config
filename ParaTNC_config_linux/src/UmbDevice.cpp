/*
 * UmbDevice.cpp
 *
 *  Created on: 13.09.2017
 *      Author: mateusz
 */

#include "UmbDevice.h"

UmbDevice::UmbDevice() : deviceID(0),
						 deviceClass(0)
{
	// TODO Auto-generated constructor stub

}

UmbDevice::~UmbDevice() {
	// TODO Auto-generated destructor stub
}

vector<DeviceChannel> * const UmbDevice::getChannels() const {
	return (vector<DeviceChannel> * const)&vChannels;
}

unsigned char UmbDevice::getDeviceClass() const {
	return deviceClass;
}

void UmbDevice::setDeviceClass(unsigned char deviceClass) {
	this->deviceClass = deviceClass;
}

unsigned char UmbDevice::getDeviceId() const {
	return deviceID;
}

void UmbDevice::setDeviceId(unsigned char deviceId) {
	deviceID = deviceId;
}

void UmbDevice::setChannels(const vector<DeviceChannel>& channels) {
	vChannels = channels;
}
