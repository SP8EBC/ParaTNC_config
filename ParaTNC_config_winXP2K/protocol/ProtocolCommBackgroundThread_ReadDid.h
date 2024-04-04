#pragma once

#include "../types/ControllerSoftwareVersion.h"
#include "../../shared/services/SrvReadDid.h"

#include "../shared/types/DidResponse.h"

struct ProtocolCommBackgroundThread_ReadDid_Context {

	// mutex declared in 'ProtocolCommBackgroundThread' 
	// to sync serial KISS protocol communication and 
	// elimiante a risk that two communication transactions
	// will be started at one time
	HANDLE hMutex;

	// DID number itself
	int didNumber;

	// callback invoked to notify that correct response from controller has been received
	VOID(*lpfnUpdateGuiCallback)(DidResponse);

	// pointer to a receive handler, which is responsible for performing low-level srial i/o
	// and decoding data received from the controller
	SrvReadDid * lpcReadDid;

	ProtocolCommBackgroundThread_ReadDid_Context::ProtocolCommBackgroundThread_ReadDid_Context()
	{
		lpfnUpdateGuiCallback = NULL;
	}
};

typedef ProtocolCommBackgroundThread_ReadDid_Context CTXPCBTRDID;
typedef ProtocolCommBackgroundThread_ReadDid_Context *  LPCTXPCBTRDID;

DWORD WINAPI ProtocolCommBackgroundThread_ReadDid(LPVOID param);