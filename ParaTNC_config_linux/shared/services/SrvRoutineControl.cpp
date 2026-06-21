/*
 * SrvRoutineControl.cpp
 *
 *  Created on: Jun 1, 2026
 *      Author: mateusz
 */

#include "SrvRoutineControl.h"

SrvRoutineControl::SrvRoutineControl () : lastResult{0u}
{
#if defined(_MSC_VER) && (_MSC_VER <= 1400)
	syncEvent = OpenEvent (EVENT_ALL_ACCESS, false, L"ServiceSyncEv");
#else
	conditionVariable = 0;
#endif
	// TODO Auto-generated constructor stub
	s = 0;
	requestedSubfunction = ROUTINE_CTRL_SUBFUNC_UNINIT;
	routineId = 0;
	responseCallback = NULL;
}

void SrvRoutineControl::sendRequest ()
{
	if (s != 0 && this->requestData.size () > 0) {
		s->transmitKissFrame (this->requestData);
	}
}

void SrvRoutineControl::waitForTransmissionDone ()
{
	if (s) {
		s->waitForTransmissionDone ();
	}
}

void SrvRoutineControl::receiveSynchronously (IService_NegativeResponseCodeCbk cbk)
{
}

SrvRoutineControl::~SrvRoutineControl ()
{
	// TODO Auto-generated destructor stub
}

bool SrvRoutineControl::startRoutine (uint16_t id, SrvRoutineControl_ResultCbk callback,
									  uint16_t wparam, uint32_t lparam)
{
	// check if all variables are set correctly
	if (this->s != NULL) {

		// if response callback pointer is set to null, the communication is idle. a TNC under test
		// is not busy on executing a diagnostic routine or any other diagnostics service
		if (responseCallback == NULL) {

			// wipe content of request buffer
			requestData.clear ();

			// service identifier
			requestData.push_back (KISS_ROUTINE_CONTROL);

			// subfunction
			requestData.push_back ((uint8_t)KISS_ROUTINE_CONTROL_SUBFUNC_START);

			// address
			requestData.push_back ((uint8_t)((id & 0x00FFu)));
			requestData.push_back ((uint8_t)((id & 0xFF00u) >> 8));

			// uint32_t parameter
			requestData.push_back ((uint8_t)((lparam & 0x000000FFu)));
			requestData.push_back ((uint8_t)((lparam & 0x0000FF00u) >> 8));
			requestData.push_back ((uint8_t)((lparam & 0x00FF0000u) >> 16));
			requestData.push_back ((uint8_t)((lparam & 0xFF000000u) >> 24));

			// uint16_t parameter
			requestData.push_back ((uint8_t)((wparam & 0x00FFu)));
			requestData.push_back ((uint8_t)((wparam & 0xFF00u) >> 8));

			responseCallback = callback;

			sendRequest ();

			return true;
		}
		else {
			// if responseCallback is set
			return false;
		}
	}
	else {
		return false;
	}
}

bool SrvRoutineControl::stopRoutine (uint16_t id, SrvRoutineControl_ResultCbk callback)
{
	// check if all variables are set correctly
	if (this->s != NULL && (responseCallback == NULL)) {
		// wipe content of request buffer
		requestData.clear ();

		// service identifier
		requestData.push_back (KISS_ROUTINE_CONTROL);

		// subfunction
		requestData.push_back ((uint8_t)KISS_ROUTINE_CONTROL_SUBFUNC_STOP);

		// address
		requestData.push_back ((uint8_t)((id & 0x00FFu)));
		requestData.push_back ((uint8_t)((id & 0xFF00u) >> 8));
		
		responseCallback = callback;

		sendRequest ();

		return true;
	}
	else {
		return false;
	}
}

bool SrvRoutineControl::requestRoutineResult (uint16_t id, SrvRoutineControl_ResultCbk callback)
{
	// check if all variables are set correctly
	if (this->s != NULL && (responseCallback == NULL)) {
		// wipe content of request buffer
		requestData.clear ();

		// service identifier
		requestData.push_back (KISS_ROUTINE_CONTROL);

		// subfunction
		requestData.push_back ((uint8_t)KISS_ROUTINE_CONTROL_SUBFUNC_RESULT);

		// address
		requestData.push_back ((uint8_t)((id & 0x00FFu)));
		requestData.push_back ((uint8_t)((id & 0xFF00u) >> 8));
		
		responseCallback = callback;

		sendRequest ();

		return true;
	}
	else {
		return false;
	}
}

void SrvRoutineControl::callback (
	const std::vector<unsigned char, std::allocator<unsigned char>> *frame)
{
	if (frame == NULL) {
		return;
	}

	// by design, if this callback is invoked, the positive, non-NRC response was received
	// please look at @link{SerialRxBackgroundWorker::worker}
	const RoutineControlSubfunction subfunc = (RoutineControlSubfunction)frame->at (2);
	uint16_t routineId = frame->at (3) | (frame->at (4) << 8); 

	RoutineControlResult result = {0};
	result.routineId = routineId;
	result.subfunction = subfunc;

	switch (subfunc) {
	case ROUTINE_CTRL_SUBFUNC_UNINIT: break;
	case ROUTINE_CTRL_SUBFUNC_REQUEST_RES: 
		result.resultCode = frame->at (3) | (frame->at (4) << 8); 
	case ROUTINE_CTRL_SUBFUNC_START:
	case ROUTINE_CTRL_SUBFUNC_STOP:
		if (responseCallback != NULL) 
		{
			responseCallback(result);
		}
		break;
	}
}
