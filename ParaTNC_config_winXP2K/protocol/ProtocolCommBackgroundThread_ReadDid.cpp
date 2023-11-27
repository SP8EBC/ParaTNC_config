#include "stdafx.h"
#include "configuration.h"
#include "Resource.h"
#include "ProtocolCommBackgroundThread_ReadDid.h"

#include "../shared/exceptions/TimeoutE.h"

#include <iostream>

DWORD WINAPI ProtocolCommBackgroundThread_ReadDid(LPVOID param)
{
	DWORD result = 1;
	
	if (param != NULL)
	{
		// cast thread entry parameter to context structure
		LPCTXPCBTRDID context = static_cast<LPCTXPCBTRDID>(param);

		// try to signalize the sync mutex
		DWORD result = WaitForSingleObject(context->mutex, CONFIG_WAITFORSINGLEOBJECT_COMM_THREADMUTEX);

		// check the result
		if (result == WAIT_OBJECT_0) 
		{
			try {
				// if mutex is signalled to this thread and we are ready to go
				std::cout << "I = ProtocolCommBackgroundThread_ReadDid, mutex signalled" << std::endl;
				
				context->readDid->sendRequestForDid((uint16_t)(context->didNumber & 0xFFFFU));

				context->readDid->receiveSynchronously();
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

			ReleaseMutex(context->mutex);
		}
		else
		{

		}
	}

	return result;
}
