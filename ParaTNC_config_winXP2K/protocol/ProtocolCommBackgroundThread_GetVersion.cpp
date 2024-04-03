#include "stdafx.h"
#include "configuration.h"
#include "ProtocolCommBackgroundThread_GetVersion.h"

#include "Resource.h"

#include <iostream>

DWORD WINAPI ProtocolCommBackgroundThread_GetVersion(LPVOID param) 
{
	DWORD result = 1;
	
	if (param != NULL)
	{
		// cast thread entry parameter to context structure
		LPCTXPCBTVER context = static_cast<LPCTXPCBTVER>(param);

		// check mandatory pointers, which must be set for this to work correctly
		if ((context->hMutex != NULL) && (context->lpcGetVersionAndId != NULL))
		{
			// try to signalize the sync mutex
			DWORD result = WaitForSingleObject(context->hMutex, CONFIG_WAITFORSINGLEOBJECT_COMM_THREADMUTEX);

			// check the result
			if (result == WAIT_OBJECT_0) 
			{
				// if mutex is signalled to this thread and we are ready to go
				std::cout << "I = ProtocolCommBackgroundThread_GetVersion, mutex signalled" << std::cout;
				
				// send request to controller
				context->lpcGetVersionAndId->sendRequest();

				context->lpcGetVersionAndId->receiveSynchronously();

				//// kiss protocol version
				context->versionAndIdResult.kissVersion = context->lpcGetVersionAndId->getProtocolVersion();
				
				//// software version 
				const std::string softwareVersion = context->lpcGetVersionAndId->getSoftwareVersion();

				// clear result structure before doing anything else
				memset(context->versionAndIdResult.softwareVersion, 0x00, 5 * sizeof(CHAR));
				memset(context->versionAndIdResult.softwareVersionW, 0x00, 5 * sizeof(WCHAR));

				// copy result from string into array of chars
				strncpy(context->versionAndIdResult.softwareVersion, softwareVersion.c_str(), 5);

				// convert array od CHAR into array of TCHAR
				mbstowcs(
					context->versionAndIdResult.softwareVersionW,
					context->versionAndIdResult.softwareVersion,
					strlen(context->versionAndIdResult.softwareVersion)
					);

				//// controller type. might be ParaMETEO or ParaTNC
				const std::string boardType = context->lpcGetVersionAndId->getBoardType();
				if (boardType == "METEO") {
					context->versionAndIdResult.type = CONTROLLER_SOFTWARE_PARAMETEO;
				}
				else if (boardType == "TNC") {
					context->versionAndIdResult.type = CONTROLLER_SOFTWARE_PARATNC;
				}

				SetDlgItemText(context->hMainWindow, IDC_EDIT_VERSION, context->versionAndIdResult.softwareVersionW);

				ReleaseMutex(context->hMutex);
			}
			else
			{
				std::cout << "E = ProtocolCommBackgroundThread_GetVersion, mutex not signalled, result: 0x"
					<< std::hex << result << std::dec << std::endl;
			}
		}
	}

	return result;
}