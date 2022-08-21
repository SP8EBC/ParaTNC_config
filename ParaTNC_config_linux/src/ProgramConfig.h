/*
 * ProgramConfig.h
 *
 *  Created on: 12.09.2017
 *      Author: mateusz
 */

#ifndef CONFIG_PROGRAMCONFIG_H_
#define CONFIG_PROGRAMCONFIG_H_

#define TIMEOUT_SECS 5

#include <string>
#include <vector>

using namespace std;

class ProgramConfig {
private:



public:
	ProgramConfig();
	virtual ~ProgramConfig();

	static int readConfigFromFile(std::string fn);
	static int manualConfig();


};

#endif /* CONFIG_PROGRAMCONFIG_H_ */
