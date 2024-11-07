#pragma once

#include "../types/ControllerSoftwareVersion.h"
#include "../../shared/services/SrvReadDid.h"

#include "../shared/types/DidResponse.h"

typedef VOID(*CBKRDIDRDY)(DidResponse, const std::vector<uint8_t> &);

struct ProtocolCommBackgroundThread_ReadDid_Context {

	// mutex declared in 'ProtocolCommBackgroundThread' 
	// to sync serial KISS protocol communication and 
	// elimiante a risk that two communication transactions
	// will be started at one time
	HANDLE hMutex;

	// DID number itself
	int didNumber;

	// callback invoked to notify that correct response from controller has been received
	CBKRDIDRDY lpfnUpdateGuiCallback;

	// calback invoked when NRC is received
	IService_NegativeResponseCodeCbk lpfnNrcCbk;

	// pointer to a receive handler, which is responsible for performing low-level srial i/o
	// and decoding data received from the controller
	SrvReadDid * lpcReadDid;

	ProtocolCommBackgroundThread_ReadDid_Context::ProtocolCommBackgroundThread_ReadDid_Context()
	{
		lpfnUpdateGuiCallback = NULL;
		lpfnNrcCbk = NULL;
	}
};

typedef ProtocolCommBackgroundThread_ReadDid_Context CTXPCBTRDID;
typedef ProtocolCommBackgroundThread_ReadDid_Context *  LPCTXPCBTRDID;

DWORD WINAPI ProtocolCommBackgroundThread_ReadDid(LPVOID param);