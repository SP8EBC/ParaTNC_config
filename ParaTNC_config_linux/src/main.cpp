
#include <iostream>
#include "serial.h"
#include "ProgramConfig.h"

using namespace std;

int main() {
#ifndef _ONLY_MANUAL_CFG
	ProgramConfig::readConfigFromFile();
#endif

#ifdef _ONLY_MANUAL_CFG
	ProgramConfig::manualConfig();
#endif

	serial s;

	s.init("/dev/ttyUSB0");

	UmbThread *t;
	t = new UmbThread(&s, ProgramConfig::getMasterId(), ProgramConfig::getDevices());
	t->serviceThread();

	delete t;

	return 0;

}


