#pragma once

#include "../types/ControllerSoftwareVersion.h"

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
}ProtocolCommBackgroundThread_GetVersion_Context;

DWORD WINAPI ProtocolCommBackgroundThread_GetVersion(LPVOID param);