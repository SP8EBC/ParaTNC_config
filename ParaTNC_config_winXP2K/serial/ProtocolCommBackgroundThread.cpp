#include "StdAfx.h"
#include "ProtocolCommBackgroundThread.h"

ProtocolCommBackgroundThread::ProtocolCommBackgroundThread(void)
{
	const BOOL serialInitResult = s.init();

	serialThread = new SRBT(&s, &callbackMap);

	srvVersionAndId.setSerialContext(&s);
}

ProtocolCommBackgroundThread::~ProtocolCommBackgroundThread(void)
{
}
