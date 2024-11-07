#include "stdafx.h"
#include "configuration.h"
#include "Resource.h"
#include "ProtocolCommBackgroundThread_ReadDid.h"

#include "../shared/exceptions/TimeoutE.h"

#include <iostream>
#include "assert.h"

DWORD WINAPI ProtocolCommBackgroundThread_ReadDid(LPVOID param)
{
	DWORD result = 1;
	
	if (param != NULL)
	{
		// cast thread entry parameter to context structure
		LPCTXPCBTRDID lpcContext = static_cast<LPCTXPCBTRDID>(param);

		// try to signalize the sync mutex
		DWORD result = WaitForSingleObject(lpcContext->hMutex, CONFIG_WAITFORSINGLEOBJECT_COMM_THREADMUTEX);

		assert(result != WAIT_ABANDONED);

		// check the result
		if (result == WAIT_OBJECT_0) 
		{
			try {
				// if mutex is signalled to this thread and we are ready to go
				std::cout << "I = ProtocolCommBackgroundThread_ReadDid, mutex signalled" << std::endl;
				
				lpcContext->lpcReadDid->sendRequestForDid((uint16_t)(lpcContext->didNumber & 0xFFFFU));

				lpcContext->lpcReadDid->receiveSynchronously(lpcContext->lpfnNrcCbk);

				// get decoded response
				DidResponse response = lpcContext->lpcReadDid->getDidResponse();

				if (lpcContext->lpfnUpdateGuiCallback != NULL)
				{
					lpcContext->lpfnUpdateGuiCallback(response, lpcContext->lpcReadDid->getRawResponse());
				}
			}
			catch (TimeoutE & ex)
			{
				//if (context->mainWindow != NULL)
				//{
				//	BOOL intres = IS_INTRESOURCE(IDS_COMM_TIMEOUT_TITLE);
				//	WORD LangID = MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT);
				//	
				//	TCHAR szTitle[64];
				//	LoadString(NULL, IDS_COMM_TIMEOUT_TITLE, szTitle, 64);
				//	
				//	TCHAR szMessage[64];
				//	LoadString(NULL, IDS_COMM_TIMEOUT_MESSAGE_GETVER, szMessage, 64);

				//	//MAKEINTRESOURCE(IDS_COMM_TIMEOUT);
				//	MessageBox(context->mainWindow, szMessage, szTitle, MB_OK | MB_ICONERROR);
				//}
			}

			ReleaseMutex(lpcContext->hMutex);
		}
		else
		{

		}
	}

	return result;
}
