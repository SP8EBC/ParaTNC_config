/*
 * AprsSymbol.h
 *
 *  Created on: Aug 28, 2022
 *      Author: mateusz
 */

#ifndef SRC_TYPES_APRSSYMBOL_H_
#define SRC_TYPES_APRSSYMBOL_H_


typedef enum AprsSymbol {
	SYMBOL_DIGI = 0,
	SYMBOL_WIDE1_DIGI = 1,
	SYMBOL_HOUSE = 2,
	SYMBOL_RXIGATE = 3,
	SYMBOL_IGATE = 4,
	SYMBOL_SAILBOAT = 5
}AprsSymbol;


#endif /* SRC_TYPES_APRSSYMBOL_H_ */
