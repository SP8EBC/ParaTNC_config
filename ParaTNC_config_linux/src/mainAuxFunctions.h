/*
 * mainAuxFunctions.h
 *
 *  Created on: Dec 27, 2025
 *      Author: mateusz
 */

#ifndef SRC_MAINAUXFUNCTIONS_H_
#define SRC_MAINAUXFUNCTIONS_H_

#include "../shared/services/SrvEraseStartupConfig.h"
#include "../shared/services/SrvGetRunningConfig.h"
#include "../shared/services/SrvSendStartupConfig.h"
#include "../shared/services/SrvReadDid.h"

#include <memory>

#include "BatchConfig_t.h"

size_t main_make_filename_prefix (std::string &callsign, std::string &api_name, char *out,
										 size_t max_out_ln);

size_t main_make_filename_prefix (std::string &callsign, std::string &api_name,
										 std::string &out);

void main_readDid (const int did, SrvReadDid &_srvReadDid, Serial &_s, pthread_mutex_t &_lock,
				   pthread_cond_t &_cond1);

std::shared_ptr<IConfigurationManager> main_readConfig (SrvGetRunningConfig &_srvRunningConfig,
														Serial &_s, pthread_mutex_t &_lock,
														pthread_cond_t &_cond1,
														std::string _fileNamePrefix);

std::shared_ptr<IConfigurationManager>
main_writeConfig (std::shared_ptr<IConfigurationManager> _configManager, SrvReadDid &_srvReadDid,
				  SrvEraseStartupConfig &_srvEraseConfig,
				  SrvSendStartupConfig &_srvSendStartupConfig, BatchConfig &_batchConfig,
				  Serial &_s, pthread_mutex_t &_lock, pthread_cond_t &_cond1);


#endif /* SRC_MAINAUXFUNCTIONS_H_ */
