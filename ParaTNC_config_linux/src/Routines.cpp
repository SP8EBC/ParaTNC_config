#include "Routines.hpp"
#include "TimeTools.h"

#include <cerrno>
#include <ctime>

// Definitions of the static synchronization members declared in Routines.hpp.
// Both primitives use the default static initializers, matching the rest of the
// application (see cond1 / lock in main.cpp).
pthread_mutex_t Routines::m_syncMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Routines::m_syncCondition = PTHREAD_COND_INITIALIZER;
bool Routines::m_responseReceived = false;
RoutineControlResult Routines::m_lastResult = {};

// How long to wait for the TNC to respond before giving up, in seconds
static const long ROUTINE_RESPONSE_TIMEOUT_SEC = 5;

bool Routines::setRtcToLocalDateTime ()
{
	const uint16_t routineId = 0x5254U;

	// get current local wall-clock date and time (this routine sets the RTC to
	// local time, hence local_time() rather than universal_time())
	boost::posix_time::ptime current = boost::posix_time::second_clock::local_time ();

	const boost::gregorian::date date = current.date ();
	const boost::posix_time::time_duration time = current.time_of_day ();

	/**
	 * \troutine_5254_rtc_time.TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
		routine_5254_rtc_time.Hours = (wparam & 0xFF00) >> 8;
		routine_5254_rtc_time.Minutes = wparam & 0xFF;
		routine_5254_rtc_time.Seconds = 0;

		routine_5254_rtc_date.Day = lparam & 0xFF;
		routine_5254_rtc_date.Month = (lparam & 0xFF00) >> 8;
		routine_5254_rtc_date.Year = (lparam & 0xFFFF0000) >> 16;
	 *
	 *
	 */

	// set a value of lparam and wparam with current local date and time
	// according to code snippet above.
	//
	// wparam layout:  bits 15..8 -> hours, bits 7..0 -> minutes (seconds are
	//                 always forced to 0 on the controller side)
	// lparam layout:  bits 7..0    -> day of month
	//                 bits 15..8   -> month (1..12)
	//                 bits 31..16  -> two-digit year, e.g. 26 for 2026
	//                 (matches the 0x5254 example in main.cpp: 0x001A0619)
	const uint8_t hours = static_cast<uint8_t> (time.hours ());
	const uint8_t minutes = static_cast<uint8_t> (time.minutes ());
	const uint8_t day = static_cast<uint8_t> (date.day ());
	const uint8_t month = static_cast<uint8_t> (date.month ());
	const uint16_t year = static_cast<uint16_t> (date.year () % 100);

	uint32_t lparam = (static_cast<uint32_t> (year) << 16) | (static_cast<uint32_t> (month) << 8) |
					  static_cast<uint32_t> (day);
	uint16_t wparam = static_cast<uint16_t> ((static_cast<uint16_t> (hours) << 8) |
											 static_cast<uint16_t> (minutes));

	std::cout << "I = Routines::setRtcToLocalDateTime, set to " << year << "-" << month << "-"
			  << day << ", " << hours << ":" << minutes << std::endl;

	// Take the lock and clear the predicate *before* the request is sent. The
	// response callback runs on the serial RX background worker thread and can
	// fire at any moment after startRoutine() returns - holding the lock first
	// guarantees the callback cannot set the flag (and signal) until we are
	// actually blocked in pthread_cond_timedwait(), so the wake-up can never be
	// lost.
	pthread_mutex_lock (&m_syncMutex);

	m_responseReceived = false;
	m_lastResult = {};

	const bool started =
		m_srvRoutineControl.startRoutine (routineId, callbackSetRtcToLocalDateTime, wparam, lparam);

	// startRoutine() returns false when the serial context is not set or another
	// routine is still pending. In that case no response will ever arrive, so we
	// must release the mutex and not block on the condition variable.
	if (!started) {
		pthread_mutex_unlock (&m_syncMutex);
		return false;
	}

	// Build an absolute deadline for pthread_cond_timedwait(). The condition
	// variable was created with the default (PTHREAD_COND_INITIALIZER) clock,
	// which is CLOCK_REALTIME, so the deadline must be taken from the same clock.
	struct timespec deadline;
	clock_gettime (CLOCK_REALTIME, &deadline);
	deadline.tv_sec += ROUTINE_RESPONSE_TIMEOUT_SEC;

	// wait for condition variable: block until the callback reports the response
	// (or bail out after the timeout so we never hang forever on unresponsive HW).
	// The loop re-checks the predicate to absorb spurious wake-ups.
	int waitResult = 0;
	while (!m_responseReceived && waitResult == 0) {
		waitResult = pthread_cond_timedwait (&m_syncCondition, &m_syncMutex, &deadline);
	}

	// capture state under the lock before releasing it
	const bool responded = m_responseReceived;
	const RoutineControlResult result = m_lastResult;

	pthread_mutex_unlock (&m_syncMutex);

	if (!responded) {
		std::cout << "E = Routines::setRtcToLocalDateTime, timeout!" << std::endl;
		// timed out (waitResult == ETIMEDOUT) or wait error
		return false;
	}

	

	// the routine finished; report success only if it was not a negative response
	return (result.wasNrc == 0);
}