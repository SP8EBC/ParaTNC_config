/*
 * DiagnosticDescription_Did.h
 *
 *  Created on: Apr 03, 2024
 *      Author: mateusz
 */
#pragma once

#include <windows.h>
#include <assert.h>
#include <string>
#include "../shared/types/DidResponse.h"

#define DIAGNOSTICDESCRIPTION_DATA_NAME_LN				16

#define DIAGNOSTICDESCRIPTION_DID_NAME_LN				32
#define DIAGNOSTICDESCRIPTION_DID_NAME_ON_LIST_LN		24

#define DIAGNOSTICDESCRIPTION_DID_DESCRIPTION_LN		256

//
struct DiagnosticDescription_DidData{
	
	DidResponse_DataSize size;

	//
	//
	//  DID scaling works as:
	//  a*x^2 + b*x + c
	//	---------------
	//			d
	//

	int scalingA;

	int scalingB;

	int scalingC;

	int scalingD;

	// this parameter has sense only if DID response has
	// DIDRESPONSE_DATASIZE_FLOAT format or it has integer
	// format and scalingD set to anything but 1 or -1
	BYTE precisionAfterDecimal;

	// name displayed as a text description of this data
	TCHAR name[DIAGNOSTICDESCRIPTION_DATA_NAME_LN];

	DiagnosticDescription_DidData() 
	{
		scalingA = 0;
		scalingB = 1;
		scalingC = 0;
		scalingD = 1;
		size = DIDRESPONSE_DATASIZE_EMPTY;
		memset(name, 0x00, sizeof(TCHAR) * DIAGNOSTICDESCRIPTION_DATA_NAME_LN);
	}

	const bool isUsed(void) const
	{
		if (size != DIDRESPONSE_DATASIZE_EMPTY)
			return true;
		else
			return false;
	}

	const bool isScaled(void) const
	{
		assert(scalingD != 0);
		if (scalingA == 0 && scalingB == 1 && scalingC == 0 && scalingD == 1) 
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	const float scale(int value) const
	{
		float out = 0.0f;
		float v = (float) value;
		assert(scalingD != 0);

		out = (scalingA*v*v + scalingB*v + scalingC) / scalingD;

		return out;
	}

	const float scale(float v) const
	{
		float out = 0.0f;
		assert(scalingD != 0);

		out = (scalingA*v*v + scalingB*v + scalingC) / scalingD;

		return out;
	}
};

//
struct DiagnosticDescription_Did {
	
	// id of this data identifier
	int id;

	TCHAR didNameOnList[DIAGNOSTICDESCRIPTION_DID_NAME_ON_LIST_LN];

	TCHAR didName[DIAGNOSTICDESCRIPTION_DID_NAME_LN];

	TCHAR didDescription[DIAGNOSTICDESCRIPTION_DID_DESCRIPTION_LN];

	DiagnosticDescription_DidData first;

	DiagnosticDescription_DidData second;

	DiagnosticDescription_DidData third;
};

typedef DiagnosticDescription_DidData DDDD;
typedef DiagnosticDescription_Did	DDD;