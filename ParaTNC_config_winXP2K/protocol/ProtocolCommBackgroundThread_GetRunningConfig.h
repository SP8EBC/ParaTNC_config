#pragma once

#include "../types/ControllerSoftwareVersion.h"
#include "../../shared/services/SrvGetRunningConfig.h"
#include "../../shared/config/decode/DecodeVer0.h"

typedef struct {

	// mutex declared in 'ProtocolCommBackgroundThread' 
	// to sync serial KISS protocol communication and 
	// elimiante a risk that two communication transactions
	// will be started at one time
	HANDLE hMutex;

	IConfigDecode * lpcConfigDecode;

	SrvGetRunningConfig * lpcGetRunningConfig;
	
	uint32_t programmingCounterFromTnc; 

}ProtocolCommBackgroundThread_GetRunningConfig_Context;

typedef ProtocolCommBackgroundThread_GetRunningConfig_Context CTXPCBTGRC;
typedef ProtocolCommBackgroundThread_GetRunningConfig_Context *  LPCTXPCBTGRC;

DWORD WINAPI ProtocolCommBackgroundThread_GetRunningConfig(LPVOID param);
