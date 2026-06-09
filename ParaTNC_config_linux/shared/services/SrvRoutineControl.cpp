/*
 * SrvRoutineControl.cpp
 *
 *  Created on: Jun 1, 2026
 *      Author: mateusz
 */

#include "SrvRoutineControl.h"

SrvRoutineControl::SrvRoutineControl () : lastResult {0u}
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

void SrvRoutineControl::callRoutine (uint16_t id, RoutineControlSubfunction subfunc,
									 uint16_t wparam, uint32_t lparam)
{
	// check if all variables are set correctly
	if (this->s != NULL) {
		// wipe content of request buffer
		requestData.clear ();

		// service identifier
		requestData.push_back (KISS_ROUTINE_CONTROL);

		// subfunction
		requestData.push_back ((uint8_t)subfunc);

		// address
		requestData.push_back ((uint8_t)((id & 0x00FFu)));
		requestData.push_back ((uint8_t)((id & 0xFF00u) >> 8));

		if (subfunc == ROUTINE_CTRL_SUBFUNC_START) {

			// uint32_t parameter
			requestData.push_back ((uint8_t)((lparam & 0x000000FFu)));
			requestData.push_back ((uint8_t)((lparam & 0x0000FF00u) >> 8));
			requestData.push_back ((uint8_t)((lparam & 0x00FF0000u) >> 16));
			requestData.push_back ((uint8_t)((lparam & 0xFF000000u) >> 24));

			// uint16_t parameter
			requestData.push_back ((uint8_t)((wparam & 0x00FFu)));
			requestData.push_back ((uint8_t)((wparam & 0xFF00u) >> 8));
		}

		sendRequest ();
	}
}

void SrvRoutineControl::callback (
	const std::vector<unsigned char, std::allocator<unsigned char>> *frame)
{
	if (frame == NULL)
	{
		return;
	}

	const RoutineControlSubfunction subfunc = (RoutineControlSubfunction)frame->at(2);

	switch (subfunc)
	{
	case ROUTINE_CTRL_SUBFUNC_START:
	case ROUTINE_CTRL_SUBFUNC_STOP:
	case ROUTINE_CTRL_SUBFUNC_REQUEST_RES:
		break;
	}


}
