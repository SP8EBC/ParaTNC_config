/*
 * LogDumper.h
 *
 *  Created on: Nov 8, 2024
 *      Author: mateusz
 */

#ifndef SRC_LOGDUMPER_H_
#define SRC_LOGDUMPER_H_

#include <string>

#include "../shared/services/SrvReadMemory.h"

class LogDumper {

	SrvReadMemory& srvReadMemory;
	pthread_cond_t& cond1;
public:
	LogDumper(SrvReadMemory& _srvReadMemory, pthread_cond_t& _cond1);
	virtual ~LogDumper();

	void dumpEventsToReport(uint32_t startAddress, uint32_t endAddress, std::string filename);
};

#endif /* SRC_LOGDUMPER_H_ */
