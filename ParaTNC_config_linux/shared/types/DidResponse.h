/*
 * DidResponse.h
 *
 *  Created on: Nov 25, 2023
 *      Author: mateusz
 */

#ifndef SHARED_TYPES_DIDRESPONSE_H_
#define SHARED_TYPES_DIDRESPONSE_H_

#include <stdint.h>
#include <string>

#define DID_RESPONSE_MAX_STRING	64

typedef enum DidResponse_DataSize {
	DIDRESPONSE_DATASIZE_EMPTY,
	DIDRESPONSE_DATASIZE_INT8,
	DIDRESPONSE_DATASIZE_INT16,
	DIDRESPONSE_DATASIZE_INT32,
	DIDRESPONSE_DATASIZE_FLOAT,
	DIDRESPONSE_DATASIZE_STRING

}DidResponse_DataSize;

typedef union DidResponse_Data {
	int8_t i8;
	int16_t i16;
	int32_t i32;
	float f;
	char str[DID_RESPONSE_MAX_STRING];
}DidResponse_Data;

typedef struct DidResponse {
	uint16_t did;

	DidResponse_DataSize firstSize;
	DidResponse_DataSize secondSize;
	DidResponse_DataSize thirdSize;

	DidResponse_Data first;
	DidResponse_Data second;
	DidResponse_Data third;
}DidResponse;

static inline const std::string DidResponse_DataSize_toString(DidResponse_DataSize ds) {
	switch (ds) {
		case DIDRESPONSE_DATASIZE_EMPTY: 	return "DATASIZE_EMPTY"; break;
		case DIDRESPONSE_DATASIZE_INT8:		return "DATASIZE_INT8"; break;
		case DIDRESPONSE_DATASIZE_INT16: 	return "DATASIZE_INT16"; break;
		case DIDRESPONSE_DATASIZE_INT32: 	return "DATASIZE_INT32"; break;
		case DIDRESPONSE_DATASIZE_FLOAT: 	return "DATASIZE_FLOAT"; break;
		case DIDRESPONSE_DATASIZE_STRING: 	return "DATASIZE_STRING"; break;
	}

	return "";
}

#endif /* SHARED_TYPES_DIDRESPONSE_H_ */
