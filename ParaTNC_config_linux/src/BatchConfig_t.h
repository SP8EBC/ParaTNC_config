/*
 * BatchConfig_t.h
 *
 *  Created on: Dec 27, 2025
 *      Author: mateusz
 */

#ifndef SRC_BATCHCONFIG_T_H_
#define SRC_BATCHCONFIG_T_H_


typedef struct BatchConfig {
	BatchConfig ()
		: defaultBatch (false), monitorMode (false), performRestart (false), readDid (false),
		  monitorDid (false), didToRead (), readConfig (false), writeConfig (false),
		  configFileToWrite ()
	{
	}

	// no diagnostic services has been selected, perform default batch
	bool defaultBatch;
	bool monitorMode;

	bool performRestart;

	bool readDid;
	bool monitorDid;
	std::string didToRead;

	bool readConfig;

	bool writeConfig;
	std::string configFileToWrite;
} BatchConfig;


#endif /* SRC_BATCHCONFIG_T_H_ */
