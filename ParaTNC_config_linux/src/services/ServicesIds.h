/*
 * ServicesIds.h
 *
 *  Created on: Aug 22, 2022
 *      Author: mateusz
 */

#ifndef SRC_SERVICES_SERVICESIDS_H_
#define SRC_SERVICES_SERVICESIDS_H_

#define KISS_DATA	 				(uint8_t) 0x00
#define KISS_GET_RUNNING_CONFIG 	(uint8_t) 0x20
#define KISS_RUNNING_CONFIG			(uint8_t) 0x70

#define KISS_GET_VERSION_AND_ID		(uint8_t) 0x21
#define KISS_VERSION_AND_ID			(uint8_t) 0x71

#define KISS_ERASE_STARTUP_CFG		(uint8_t) 0x22
#define KISS_ERASE_STARTUP_CFG_RESP	(uint8_t) 0x72

#define KISS_PROGRAM_STARTUP_CFG		(uint8_t) 0x23
#define KISS_PROGRAM_STARTUP_CFG_RESP	(uint8_t) 0x73


#endif /* SRC_SERVICES_SERVICESIDS_H_ */
