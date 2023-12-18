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
		if ((context->mutex != NULL) && (context->getVersionAndId != NULL))
		{
			// try to signalize the sync mutex
			DWORD result = WaitForSingleObject(context->mutex, CONFIG_WAITFORSINGLEOBJECT_COMM_THREADMUTEX);

			// check the result
			if (result == WAIT_OBJECT_0) 
			{
				// if mutex is signalled to this thread and we are ready to go
				std::cout << "I = ProtocolCommBackgroundThread_GetVersion, mutex signalled" << std::cout;
				
				// send request to controller
				context->getVersionAndId->sendRequest();

				context->getVersionAndId->receiveSynchronously();

				//// kiss protocol version
				context->srvVersionAndIdResult.kissVersion = context->getVersionAndId->getProtocolVersion();
				
				//// software version 
				const std::string softwareVersion = context->getVersionAndId->getSoftwareVersion();

				// clear result structure before doing anything else
				memset(context->srvVersionAndIdResult.softwareVersion, 0x00, 5 * sizeof(CHAR));
				memset(context->srvVersionAndIdResult.softwareVersionW, 0x00, 5 * sizeof(WCHAR));

				// copy result from string into array of chars
				strncpy(context->srvVersionAndIdResult.softwareVersion, softwareVersion.c_str(), 5);

				// convert array od CHAR into array of WCHAR
				mbstowcs(
					context->srvVersionAndIdResult.softwareVersionW,
					context->srvVersionAndIdResult.softwareVersion,
					strlen(context->srvVersionAndIdResult.softwareVersion)
					);

				//// controller type. might be ParaMETEO or ParaTNC
				const std::string boardType = context->getVersionAndId->getBoardType();
				if (boardType == "METEO") {
					context->srvVersionAndIdResult.type = CONTROLLER_SOFTWARE_PARAMETEO;
				}
				else if (boardType == "TNC") {
					context->srvVersionAndIdResult.type = CONTROLLER_SOFTWARE_PARATNC;
				}

				SetDlgItemText(context->mainWindow, IDC_EDIT_VERSION, context->srvVersionAndIdResult.softwareVersionW);

				ReleaseMutex(context->mutex);
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