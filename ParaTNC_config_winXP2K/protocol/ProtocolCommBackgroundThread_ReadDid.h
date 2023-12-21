#pragma once

#include "../types/ControllerSoftwareVersion.h"
#include "../../shared/services/SrvReadDid.h"

typedef struct {

	// mutex declared in 'ProtocolCommBackgroundThread' 
	// to sync serial KISS protocol communication and 
	// elimiante a risk that two communication transactions
	// will be started at one time
	HANDLE hMutex;

	// DID number itself
	int didNumber;

	SrvReadDid * lpcReadDid;
}ProtocolCommBackgroundThread_ReadDid_Context;

typedef ProtocolCommBackgroundThread_ReadDid_Context CTXPCBTRDID;
typedef ProtocolCommBackgroundThread_ReadDid_Context *  LPCTXPCBTRDID;

DWORD WINAPI ProtocolCommBackgroundThread_ReadDid(LPVOID param);