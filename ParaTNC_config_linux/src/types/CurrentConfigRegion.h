/*
 * CurrentConfigRegion.h
 *
 *  Created on: Aug 24, 2022
 *      Author: mateusz
 */

#ifndef SRC_TYPES_CURRENTCONFIGREGION_H_
#define SRC_TYPES_CURRENTCONFIGREGION_H_

typedef enum CurrentConfigRegion {
	CURRENT_CONFIG_FIRST = 0x01,
	CURRENT_CONFIG_SECOND = 0x02,
	CONTENT = 0xAB
} CurrentConfigRegion;

#endif /* SRC_TYPES_CURRENTCONFIGREGION_H_ */
