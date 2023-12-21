#pragma once

#include "../types/ControllerSoftwareVersion.h"
#include "../../shared/services/SrvGetVersionAndId.h"
/*
 * Context data passed into thread entry point
 *
 */
typedef struct ProtocolCommBackgroundThread_GetVersion_Context {
	// response from the controller
	CSV versionAndIdResult;

	// handle to main dialog which shall be updated
	// after successfull communication with controller
	HWND hMainWindow;
	
	// handle to main dialog which might be updated
	// after successfull comm if this dialog is created
	HWND hEditCodeplugWindow;

	// mutex declared in 'ProtocolCommBackgroundThread' 
	// to sync serial KISS protocol communication and 
	// elimiante a risk that two communication transactions
	// will be started at one time
	HANDLE hMutex;

	// pointer to a class instance, which is responsible for
	// serial communication itself
	SrvGetVersionAndId * lpcGetVersionAndId;
}ProtocolCommBackgroundThread_GetVersion_Context;

typedef ProtocolCommBackgroundThread_GetVersion_Context CTXPCBTVER;
typedef ProtocolCommBackgroundThread_GetVersion_Context * LPCTXPCBTVER;

DWORD WINAPI ProtocolCommBackgroundThread_GetVersion(LPVOID param);