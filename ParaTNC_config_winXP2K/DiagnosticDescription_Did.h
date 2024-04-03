/*
 * DiagnosticDescription_Did.h
 *
 *  Created on: Apr 03, 2024
 *      Author: mateusz
 */
#pragma once

#include <windows.h>
#include <string>
#include "../shared/types/DidResponse.h"

#define DIAGNOSTICDESCRIPTION_DATA_NAME_LN				16

#define DIAGNOSTICDESCRIPTION_DID_NAME_LN				32
#define DIAGNOSTICDESCRIPTION_DID_NAME_ON_LIST_LN		16

#define DIAGNOSTICDESCRIPTION_DID_DESCRIPTION_LN		64

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