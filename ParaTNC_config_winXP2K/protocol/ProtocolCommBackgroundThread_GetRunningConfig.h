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

	// Poiner to an instance of the class which will decode configuration
	// taken from the controller, validate it and then store
	IConfigDecode * lpcConfigDecode;

	// This is a pointer to application global instance of a vector
	// which holds configuration which is edited by a user
	std::vector<uint8_t> * lpvEditConfig;

	// callback invoked after new configuration data has been successfully
	// obtained from the tnc and then positively validated 
	VOID(*lpfnEditConfigUpdateCallback)(VOID);

	// service handler responsible for performing serial I/O using KISS protocol. 
	// internally this class has a pointer to another, low level class which
	// handles sending and receiving binary data through serial port.
	SrvGetRunningConfig * lpcGetRunningConfig;
	
	// a value of programming counter of currently running TNC configuration
	uint32_t programmingCounterFromTnc; 

}ProtocolCommBackgroundThread_GetRunningConfig_Context;

typedef ProtocolCommBackgroundThread_GetRunningConfig_Context CTXPCBTGRC;
typedef ProtocolCommBackgroundThread_GetRunningConfig_Context *  LPCTXPCBTGRC;

DWORD WINAPI ProtocolCommBackgroundThread_GetRunningConfig(LPVOID param);
