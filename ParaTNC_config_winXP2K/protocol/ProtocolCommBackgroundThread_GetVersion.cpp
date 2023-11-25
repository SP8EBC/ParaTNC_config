#include "stdafx.h"
#include "configuration.h"
#include "Resource.h"
#include "ProtocolCommBackgroundThread_GetVersion.h"

#include "../shared/exceptions/TimeoutE.h"

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
				// receiveSynchronously method may throw an exception
				try {
					// if mutex is signalled to this thread and we are ready to go
					std::cout << "I = ProtocolCommBackgroundThread_GetVersion, mutex signalled" << std::endl;
					
					// send request to controller
					context->getVersionAndId->sendRequest();

					// recieve a response from controller and use a callback internally here
					context->getVersionAndId->receiveSynchronously();

					// get results obtained from a controller
					const std::string boardType = context->getVersionAndId->getBoardType();
					const std::string softwareVersion = context->getVersionAndId->getSoftwareVersion();
					const char protocolVersion = context->getVersionAndId->getProtocolVersion();

					// store board type in context
					if (boardType == "METEO")
					{
						context->srvVersionAndIdResult.type = CONTROLLER_SOFTWARE_PARAMETEO;
					}
					else if (boardType == "TNC")
					{
						context->srvVersionAndIdResult.type = CONTROLLER_SOFTWARE_PARATNC;
					}
					else 
					{
						; // unknown and unsupported controller type?
					}
					
					// store kiss protocol version
					context->srvVersionAndIdResult.kissVersion = protocolVersion;

					// store software version
					strncpy_s(context->srvVersionAndIdResult.softwareVersion, 5, softwareVersion.c_str(), 4);
				
					// convert software version string to wide char string
					MultiByteToWideChar(
						CP_ACP, 
						MB_PRECOMPOSED, 
						context->srvVersionAndIdResult.softwareVersion, 
						4, 
						context->srvVersionAndIdResult.softwareVersionW, 
						4);

					if (context->mainWindow != NULL)
					{
						LPWSTR szText = context->srvVersionAndIdResult.softwareVersionW;

						// check if main window exist (it should be)
						BOOL isMainWndExist = IsWindow(context->mainWindow);

						if (isMainWndExist)
						{
							// update main dialog window with a software version
							SetDlgItemText(context->mainWindow, IDC_EDIT_VERSION, szText);
						}
						else
						{
							std::cout << "E = ProtocolCommBackgroundThread_GetVersion, main dialog doesnt exist!";
						}
					}
				}
				catch (TimeoutE & ex)
				{
					if (context->mainWindow != NULL)
					{
						BOOL intres = IS_INTRESOURCE(IDS_COMM_TIMEOUT_TITLE);
						WORD LangID = MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT);
						
						TCHAR szTitle[64];
						LoadString(NULL, IDS_COMM_TIMEOUT_TITLE, szTitle, 64);
						
						TCHAR szMessage[64];
						LoadString(NULL, IDS_COMM_TIMEOUT_MESSAGE_GETVER, szMessage, 64);

						//MAKEINTRESOURCE(IDS_COMM_TIMEOUT);
						MessageBox(context->mainWindow, szMessage, szTitle, MB_OK | MB_ICONERROR);
					}
				}


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