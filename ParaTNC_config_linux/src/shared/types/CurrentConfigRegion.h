/*
 * CurrentConfigRegion.h
 *
 *  Created on: Aug 24, 2022
 *      Author: mateusz
 */

#ifndef SRC_TYPES_CURRENTCONFIGREGION_H_
#define SRC_TYPES_CURRENTCONFIGREGION_H_

typedef enum CurrentConfigRegion {
	CURRENT_CONFIG_DEFAULT = 0x00,
	CURRENT_CONFIG_FIRST = 0x01,
	CURRENT_CONFIG_SECOND = 0x02,
	CURRENT_CONFIG_CONTENT = 0xAB,
	UNDEF = 0xFF
} CurrentConfigRegion;

#endif /* SRC_TYPES_CURRENTCONFIGREGION_H_ */
