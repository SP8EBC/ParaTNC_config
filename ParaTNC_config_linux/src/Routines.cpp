#include "Routines.hpp"
#include "TimeTools.h"

bool Routines::setRtcToLocalDateTime()
{   
    const uint16_t routineId = 0x5354U;

    // get currrent timestamp
    boost::posix_time::ptime current = boost::posix_time::second_clock::universal_time();
        
    /**
     * 	routine_5254_rtc_time.TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
        routine_5254_rtc_time.Hours = (wparam & 0xFF00) >> 8;
        routine_5254_rtc_time.Minutes = wparam & 0xFF;
        routine_5254_rtc_time.Seconds = 0;

        routine_5254_rtc_date.Day = lparam & 0xFF;
        routine_5254_rtc_date.Month = (lparam & 0xFF00) >> 8;
        routine_5254_rtc_date.Year = (lparam & 0xFFFF0000) >> 16;
     * 
     * 
     */

     // TODO: set a value of lparam and wparam with current local date and time
     // according to code snippet above
     uint32_t lparam = 0;
     uint16_t wparam = 0;

     m_srvRoutineControl.startRoutine(routineId, callbackSetRtcToLocalDateTime, wparam, lparam);

     // TODO: wait for condition variable
}
