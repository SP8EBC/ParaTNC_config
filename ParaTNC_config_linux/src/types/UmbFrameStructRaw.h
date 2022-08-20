/*
 * UmbFrameStruct.h
 *
 *  Created on: 19.09.2017
 *      Author: mateusz
 */

#ifndef TYPES_UMBFRAMESTRUCTRAW_H_
#define TYPES_UMBFRAMESTRUCTRAW_H_


struct UmbFrameRaw {
	unsigned char protVersion;

	unsigned char slaveId;
	unsigned char slaveClass;

	unsigned char ln;		// LN pomiędzy STX i ETX zgodnie ze standardem
	unsigned char bytesRxed;

	unsigned char cmdId;

	unsigned char *content;		// payload

	unsigned short checksumRxed;
	bool chceksumCorrectRX;


};



#endif /* TYPES_UMBFRAMESTRUCTRAW_H_ */
