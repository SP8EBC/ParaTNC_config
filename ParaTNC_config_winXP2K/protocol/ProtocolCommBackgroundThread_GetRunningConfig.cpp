#include "stdafx.h"
#include "configuration.h"
#include "Resource.h"
#include "ProtocolCommBackgroundThread_GetRunningConfig.h"

#include "../shared/exceptions/TimeoutE.h"

#include <iostream>

DWORD WINAPI ProtocolCommBackgroundThread_GetRunningConfig(LPVOID param)
{
	DWORD result = 1;
	
	if (param != NULL)
	{
		// cast thread entry parameter to context structure
		LPCTXPCBTGRC lpcContext = static_cast<LPCTXPCBTGRC>(param);

		// try to signalize the sync mutex
		DWORD result = WaitForSingleObject(lpcContext->hMutex, CONFIG_WAITFORSINGLEOBJECT_COMM_THREADMUTEX);

		// check the result
		if (result == WAIT_OBJECT_0) 
		{
			// if mutex is signalled to this thread and we are ready to go
			std::cout << "I = ProtocolCommBackgroundThread_GetRunningConfig, mutex signalled" << std::endl;
			
			// send request to controller
			lpcContext->lpcGetRunningConfig->sendRequest();

			// recieve a response from controller and use a callback internally here
			lpcContext->lpcGetRunningConfig->receiveSynchronously();

			// check if CRC checksum is correct
			if (lpcContext->lpcGetRunningConfig->isValidatedOk()) 
			{
				// make a reference to data received from the controller
				const std::vector<uint8_t>& dataFromTnc = lpcContext->lpcGetRunningConfig->getConfigurationData();

				// create decode class and put a data received from controller there
				lpcContext->lpcConfigDecode = new DecodeVer0(dataFromTnc);

				// store programming cointer of current running config in the controller
				lpcContext->programmingCounterFromTnc = lpcContext->lpcConfigDecode->getProgrammingCounter();

				// erase all previous content of the vector with config edited by a user
				lpcContext->lpvEditConfig->clear();

				// copy content received from the controller into application global 
				lpcContext->lpvEditConfig->insert(
								lpcContext->lpvEditConfig->begin(),
								dataFromTnc.begin(),
								dataFromTnc.end());
				
				delete lpcContext->lpcConfigDecode;

				// execute callback if it is set
				if (lpcContext->lpfnEditConfigUpdateCallback != NULL)
				{
					lpcContext->lpfnEditConfigUpdateCallback();
				}
			}
			else
			{
				std::cout << "E = ProtocolCommBackgroundThread_GetRunningConfig, configuration data corrupted!" << std::endl;
		
			}
			
		}
		else
		{
		}
	}

	return result;
}