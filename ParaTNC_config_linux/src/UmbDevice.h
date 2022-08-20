/*
 * UmbDevice.h
 *
 *  Created on: 13.09.2017
 *      Author: mateusz
 */

#ifndef CONFIG_UMBDEVICE_H_
#define CONFIG_UMBDEVICE_H_

#include <vector>
#include "../types/DeviceChannel.h"

using namespace std;

class UmbDevice {

private:
	unsigned char deviceID;
	unsigned char deviceClass;

	vector<DeviceChannel> vChannels;

public:
	UmbDevice();
	virtual ~UmbDevice();

	vector<DeviceChannel> * const getChannels() const;
	void setChannels(const vector<DeviceChannel>& channels);

	unsigned char getDeviceClass() const;
	void setDeviceClass(unsigned char deviceClass);

	unsigned char getDeviceId() const;
	void setDeviceId(unsigned char deviceId);
};

#endif /* CONFIG_UMBDEVICE_H_ */
