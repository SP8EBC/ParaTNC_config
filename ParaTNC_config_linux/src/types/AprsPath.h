/*
 * AprsPath.h
 *
 *  Created on: Aug 28, 2022
 *      Author: mateusz
 */

#ifndef SRC_TYPES_APRSPATH_H_
#define SRC_TYPES_APRSPATH_H_


typedef enum AprsPath {
	PATH_NOPATH	= 0,
	PATH_WIDE11 = 1,
	PATH_WIDE21 = 2,
	PATH_WIDE11_WIDE21 = 3,
	PATH_WIDE22 = 4
}AprsPath;


#endif /* SRC_TYPES_APRSPATH_H_ */
