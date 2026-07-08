#ifndef D40B84E7_7B9E_4417_B517_12F945625E7C
#define D40B84E7_7B9E_4417_B517_12F945625E7C

#include "../shared/services/SrvRoutineControl.h"

#include <pthread.h>

class Routines {
	SrvRoutineControl &m_srvRoutineControl;
 
	/**
	 * Mutex guarding the synchronization primitives below. It is locked by the
	 * thread which starts a routine (before the wait) and by the serial RX
	 * background worker thread from within the result callback.
	 */
	static pthread_mutex_t m_syncMutex;
 
	/**
	 * Condition variable used to block the thread which requested a routine until
	 * the TNC sends back a response. The response is delivered asynchronously on
	 * the serial RX background worker thread through @link{callbackSetRtcToLocalDateTime}.
	 */
	static pthread_cond_t m_syncCondition;
 
	/**
	 * Predicate protecting against spurious and lost wake-ups. Set to false right
	 * before a routine is started and to true once the response callback fires.
	 * Only ever touched while m_syncMutex is held.
	 */
	static bool m_responseReceived;
 
	/**
	 * Result delivered by the last finished routine. Only valid once
	 * @link{m_responseReceived} is true. Only ever touched while m_syncMutex is held.
	 */
	static RoutineControlResult m_lastResult;
 
	/**
	 * Result callback invoked by SrvRoutineControl (on the serial RX background
	 * worker thread) once a positive, non-NRC response to the routine is received.
	 * It stores the result and wakes up the thread waiting inside
	 * @link{setRtcToLocalDateTime}.
	 *
	 * Must be static so it can be passed as a plain C function pointer
	 * (SrvRoutineControl_ResultCbk) to SrvRoutineControl::startRoutine.
	 */
	static void callbackSetRtcToLocalDateTime(RoutineControlResult result)
	{
		pthread_mutex_lock(&m_syncMutex);
 
		m_lastResult = result;
		m_responseReceived = true;
 
		// unblock the thread waiting inside setRtcToLocalDateTime(). Signalling
		// while holding the mutex is valid POSIX and avoids racing the predicate.
		pthread_cond_signal(&m_syncCondition);
 
		pthread_mutex_unlock(&m_syncMutex);
	}


  public:
	Routines (SrvRoutineControl &srvRoutineControl) : m_srvRoutineControl{srvRoutineControl} { ; }

    bool setRtcToLocalDateTime();
};

#endif /* D40B84E7_7B9E_4417_B517_12F945625E7C */
