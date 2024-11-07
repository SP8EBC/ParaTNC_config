/*
 * AuxStuff.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author: mateusz
 */

#include "stdafx.h"
#include "AuxStuff.h"


#include "Resource.h"

AuxStuff::AuxStuff() {
	// TODO Auto-generated constructor stub

}

AuxStuff::~AuxStuff() {
	// TODO Auto-generated destructor stub
}

int AuxStuff::getResourceIdForNrc(uint16_t nrc) {
	int out = 0;
	
	switch (nrc)
	{
	case NRC_GENERAL_REJECT:						out = IDS_NRC_GENERAL_REJECT; break;
	case NRC_SERVICE_NOT_SUPPORTED:					out = IDS_NRC_SERVICE_NOT_SUPPORTED; break;
	case NRC_SUBFUNCTION_NOT_SUPPORTED:				out = IDS_NRC_SUBFUNCTION_NOT_SUPPORTED; break;
	case NRC_INCORRECT_MESSAGE_LENGTH_OR_FORMAT:	out = IDS_NRC_INCORRECT_MESSAGE_LN_FORMAT; break;
	case NRC_RESPONSE_TOO_LONG:						out = IDS_NRC_RESPONSE_TOO_LONG; break;
	case NRC_BUSY_REPEAT_REQUEST:					out = IDS_NRC_REPEAT_REQUEST; break;
	case NRC_CONDITIONS_NOT_CORRECT:				out = IDS_NRC_CONDITIONS_NOT_CORRECT; break;
	case NRC_REQUEST_SEQUENCE_ERROR:				out = IDS_NRC_REQUEST_SEQ_ERROR; break;
	case NRC_REQUEST_OUT_OF_RANGE:					out = IDS_NRC_REQ_OUT_OF_RANGE; break;
	case NRC_SECURITY_ACCESS_DENIED:				out = IDS_NRC_SECURITY_ACCESS_DENIED; break;
	case NRC_INVALID_KEY:							out = IDS_NRC_INVALID_KEY; break;
	case NRC_EXCEED_NUMBER_OF_ATTEMPTS:				out = IDS_NRC_EXCEED_NUMBER_OF_ATEMPTS; break;
	case NRC_UPLOAD_DOWNLOAD_NOT_ACCEPTED:			out = IDS_NRC_DOWNLOAD_NOT_ACCEPTED; break;
	case NRC_GENERAL_PROGRAMMING_FAIL:				out = IDS_NRC_GENERAL_PROGRAMMING_FAIL; break;


	}

	return out;
}

