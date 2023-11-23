#pragma once

#include <windows.h>
#include <map>

#include "ProtocolCommBackgroundThread_GetVersion.h"

#include "../serial/Serial.h"
#include "../serial/SerialRxBackgroundThread.h"

#include "../../shared/services/SrvGetVersionAndId.h"
#include "../../shared/kiss_communication_service_ids.h"

typedef std::map<uint8_t, IService*> CBKMAP;

/**
 * This class wraps KISS protocol communication and
 * works as a bridge between classes, which implements
 * KISS protocol itself ("../shared/services/" dir)
 * and the application GUI. 
 *
 * All serial IO is done in background thread, no to
 * lock main UI thread
 */
class ProtocolCommBackgroundThread
{
private:
	// Serial port
	SERIAL s;
	LPSRBT serialThread;

	// map with service callbacks
	CBKMAP callbackMap;

	// services
	SrvGetVersionAndId srvVersionAndId;


public:

	// all methods with names startig from 'comm' performs serial I/O
	// asynchronously in background thread. each of them stored result 
	// obtained from controller in some private structure or another object.
	// in most of cases it also updates GUI elements, like text edits etc.

	// all methods with names starting from 'get' only returns data which 
	// had been received from the controller before. 

	// gets software and protocol version from the controller.
	// updates Edit Controls on Main dialog and on Edit Codeplug
	// dialog if it has been created. information returned by
	// the controller are stored 
	BOOL commVersionAndUpdateGui(HANDLE mainWindow, HANDLE editCodeplugWindow);

	BOOL getVersion(LPCSV p);

	ProtocolCommBackgroundThread(void);
	~ProtocolCommBackgroundThread(void);
};
