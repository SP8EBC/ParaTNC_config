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

class IConfigDecode {
public:
	virtual ~IConfigDecode() {
		;
	}

	virtual bool getCallsign(std::string & call) = 0;
	virtual uint8_t getCallsign() = 0;
	virtual float getLatitude() = 0;
	virtual bool getNorS() = 0;
	virtual float getLongitude() = 0;
	virtual bool getEorW() = 0;
};

#endif /* SRC_CONFIG_DECODE_ICONFIGDECODE_H_ */
