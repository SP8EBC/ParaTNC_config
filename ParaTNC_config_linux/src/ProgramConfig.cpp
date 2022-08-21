/*
 * ProgramConfig.cpp
 *
 *  Created on: 12.09.2017
 *      Author: mateusz
 */

#include "ProgramConfig.h"

#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <libconfig.h++>


using namespace libconfig;

ProgramConfig::ProgramConfig() {
	// TODO Auto-generated constructor stub

}

ProgramConfig::~ProgramConfig() {
	// TODO Auto-generated destructor stub
}

int ProgramConfig::manualConfig() {

	return 0;



}

int ProgramConfig::readConfigFromFile(std::string fn) {
#ifndef _ONLY_MANUAL_CFG
	Config cLibConfig;

	try {
		cLibConfig.readFile(fn.c_str());
		cout << "--- libconfig++: Otwieram plik konfiguracyjny..." << endl;
	}

	catch(const FileIOException &ex)
	{
		printf("--- libconfig++: Nie mozna otworzyc pliku!!!\r\n");
		return -1;
	}
	catch(const ParseException &ex) {
		printf("--- libconfig++: Blad parsowania pliku wejsciowego!!!");
		return -2;
	}

	Setting &rRoot = cLibConfig.getRoot();

	return 0;
#endif
}

