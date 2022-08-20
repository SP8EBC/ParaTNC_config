/*
 * ChannelType.h
 *
 *  Created on: 20.09.2017
 *      Author: mateusz
 */

#ifndef TYPES_CHANNELTYPE_H_
#define TYPES_CHANNELTYPE_H_



enum ChannelType {
	NL = 0x00,
	UNSIGNED_CHAR = 0x10,
	SIGNED_CHAR = 0x11,
	UNSIGNED_SHORT = 0x12,
	SIGNED_SHORT = 0x13,
	UNSIGNED_INT = 0x14,
	SIGNED_INT = 0x15,
	FLOAT = 0x16,
	DOUBLE = 0x17
};

#endif /* TYPES_CHANNELTYPE_H_ */
