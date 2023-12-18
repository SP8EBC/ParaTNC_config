#include "stdafx.h"
#include "configuration.h"
#include "ProtocolCommBackgroundThread_GetVersion.h"

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