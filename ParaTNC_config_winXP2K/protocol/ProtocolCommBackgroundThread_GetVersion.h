#pragma once

#include "../types/ControllerSoftwareVersion.h"
#include "../../shared/services/SrvGetVersionAndId.h"
/*
 * Context data passed into thread entry point
 *
 */
typedef struct ProtocolCommBackgroundThread_GetVersion_Context {
	// response from the controller
	CSV srvVersionAndIdResult;

	// handle to main dialog which shall be updated
	// after successfull communication with controller
	HANDLE mainWindow;
	
	// handle to main dialog which might be updated
	// after successfull comm if this dialog is created
	HANDLE editCodeplugWindow;

	// mutex declared in 'ProtocolCommBackgroundThread' 
	// to sync serial KISS protocol communication and 
	// elimiante a risk that two communication transactions
	// will be started at one time
	HANDLE mutex;

	// pointer to a class instance, which is responsible for
	// serial communication itself
	SrvGetVersionAndId * getVersionAndId;
}ProtocolCommBackgroundThread_GetVersion_Context;

typedef ProtocolCommBackgroundThread_GetVersion_Context CTXPCBTVER;
typedef ProtocolCommBackgroundThread_GetVersion_Context * LPCTXPCBTVER;

DWORD WINAPI ProtocolCommBackgroundThread_GetVersion(LPVOID param);