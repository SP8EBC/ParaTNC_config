/*
 * ValidateVer0.cpp
 *
 *  Created on: Aug 27, 2022
 *      Author: mateusz
 */

#include "stdafx.h"

#include "ValidateVer0.h"
#include "../../crc/crc_.h"

#include <iostream>
#include <vector>

ValidateVer0::~ValidateVer0() {
	// TODO Auto-generated destructor stub
}

bool ValidateVer0::checkValidate(std::vector<uint8_t> & dataFromTnc) {

	uint32_t calculatedCrc = 0;

	// last four bytes of a configuration blocks holds the CRC32 value itself.
	// four bytes before CRC is used to store programming timestamp,
	// which is not included into CRC calculation.
	// four bytes AFTER a crc is not used at all and is kept due to
	// STM32L4xx target limitations which require 64 bit aligned write block size
	uint32_t crcAreaLn = (uint32_t)(dataFromTnc.size() - 12) & 0x7FFFFFFFU;

	// data are stored in litte endian order
	uint32_t crcFromFrame = 	(dataFromTnc.at(crcAreaLn + 4)) |
								(dataFromTnc.at(crcAreaLn + 5) << 8) |
								(dataFromTnc.at(crcAreaLn + 6) << 16) |
								(dataFromTnc.at(crcAreaLn + 7) << 24);

#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	calculatedCrc = calcCRC32std(&dataFromTnc[0], crcAreaLn, 0x04C11DB7, 0xFFFFFFFF, 0, 0, 0);
#else
	calculatedCrc = calcCRC32std(dataFromTnc.data(), crcAreaLn, 0x04C11DB7, 0xFFFFFFFF, 0, 0, 0);
#endif
	std::cout << "D = ValidateVer0::checkValidate, calculatedCrc: 0x" << std::hex << (int)calculatedCrc  << ", frame crcFromFrame: 0x" << crcFromFrame << std::dec << std::endl;

	if (calculatedCrc == crcFromFrame) {
		return true;
	}

	return false;
}
