/*
 * crc_.h
 *
 *  Created on: Aug 28, 2022
 *      Author: mateusz
 */

#ifndef SRC_CRC__H_
#define SRC_CRC__H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t reflect8(uint8_t val);
uint32_t reflect32(uint32_t val);

uint32_t calcCRC32stm(const void *data, uint32_t len,
   uint32_t poly, uint32_t seed, uint32_t initCRC, uint32_t inR, uint32_t outR);

uint32_t calcCRC32std(const void *data, uint32_t len,
   uint32_t poly, uint32_t seed, uint32_t initCRC, uint32_t inR, uint32_t outR);

#ifdef __cplusplus
}
#endif

#endif /* SRC_CRC__H_ */
