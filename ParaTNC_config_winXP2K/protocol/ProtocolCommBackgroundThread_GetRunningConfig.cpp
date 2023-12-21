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
				lpcContext->lpcConfigDecode = new DecodeVer0(lpcContext->lpcGetRunningConfig->getConfigurationData());

				lpcContext->programmingCounterFromTnc = lpcContext->lpcConfigDecode->getProgrammingCounter();

				std::string beaconDescription;
				lpcContext->lpcConfigDecode->getDescritpion(beaconDescription);

				const float latitude = lpcContext->lpcConfigDecode->getLatitude();
				const float longitude = lpcContext->lpcConfigDecode->getLongitude();

				std::cout << "I = beaconDescription: " << beaconDescription << std::endl;
				std::cout << "I = latitude: " << latitude << std::endl;
				std::cout << "I = longitude: " << longitude << std::endl;
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