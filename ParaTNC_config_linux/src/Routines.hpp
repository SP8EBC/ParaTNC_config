#ifndef D40B84E7_7B9E_4417_B517_12F945625E7C
#define D40B84E7_7B9E_4417_B517_12F945625E7C

#include "../shared/services/SrvRoutineControl.h"

class Routines {
	SrvRoutineControl &m_srvRoutineControl;

    static void callbackSetRtcToLocalDateTime(RoutineControlResult result)
    {
        // TODO: signalize condition variable to unblock waiting
    }

    // TODO: condition variable

  public:
	Routines (SrvRoutineControl &srvRoutineControl) : m_srvRoutineControl{srvRoutineControl} { ; }

    bool setRtcToLocalDateTime();
};

#endif /* D40B84E7_7B9E_4417_B517_12F945625E7C */
