
#include <iostream>
#include "serial.h"
#include "ProgramConfig.h"

using namespace std;

int main() {
#ifndef _ONLY_MANUAL_CFG
	//ProgramConfig::readConfigFromFile("");
#endif

#ifdef _ONLY_MANUAL_CFG
	ProgramConfig::manualConfig();
#endif

	serial s;

	s.init("/dev/ttyS0", B9600);


	return 0;

}


