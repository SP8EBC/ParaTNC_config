/*
 * SrvRoutineControl.cpp
 *
 *  Created on: Jun 1, 2026
 *      Author: mateusz
 */

#include "SrvRoutineControl.h"

SrvRoutineControl::SrvRoutineControl ()
{
#if defined (_MSC_VER) && (_MSC_VER <= 1400)
	syncEvent = OpenEvent(EVENT_ALL_ACCESS, false, L"ServiceSyncEv");
#else
	conditionVariable = 0;
#endif
	// TODO Auto-generated constructor stub
	s = 0;
}

void SrvRoutineControl::sendRequest ()
{
	if (s != 0 && this->requestData.size() > 0) {
		s->transmitKissFrame(this->requestData);
	}
}


void SrvRoutineControl::waitForTransmissionDone ()
{
	if (s) {
		s->waitForTransmissionDone();
	}
}

void SrvRoutineControl::receiveSynchronously (IService_NegativeResponseCodeCbk cbk)
{
}

SrvRoutineControl::~SrvRoutineControl ()
{
	// TODO Auto-generated destructor stub
}

void SrvRoutineControl::callback (
	const std::vector<unsigned char, std::allocator<unsigned char>> *frame)
{
}
