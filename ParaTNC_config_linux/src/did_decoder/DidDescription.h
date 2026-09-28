#ifndef C75378C7_0FE1_4550_A586_D719AAA0E213
#define C75378C7_0FE1_4550_A586_D719AAA0E213

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Description of single data variable within
 * single data-by-id response. One DID can return one,
 * two or three variable.
 *
 * Each variable can be rescaled from raw value returned by
 * the controller into physical one (of course except a string).
 * Recalculation formula is:
 *
 *      scalingA * R^2 + scalingB * R + scalingC
 * P =  -----------------------------------------
 *                       scalingD
 *
 * where R is raw value from the controller and P is physical
 *
 * @note Type of each variable (like int8, int16, int32...)
 * is returned from a controller with a value. So this
 * struct *doesn't contain* any member like DidResponse_DataSize
 * from 'shared/types/DidResponse.h' because it is not required
 * in this context.
 */
struct DidDescriptionSingleVariable {
	int32_t scalingA;
	int32_t scalingB;
	int32_t scalingC;
	int32_t scalingD;
	std::string name;
	std::string unit;
};

/**
 * @brief This struct holds a description of single Data-by-IDentifier. Its meaningful name,
 * some description of what the data is about, how many variables are returned and theirs type,
 * and recalculation coefficient.
 *
 * Values stored in an instance of this struct are read from INI file
 */
struct DidDescription {
	uint16_t id;
	std::string shortName;
	std::string longerDescription;

	/**
	 * This might be empty for a DID which returns a string
	 */
	std::vector<DidDescriptionSingleVariable> variables;
};

#endif /* C75378C7_0FE1_4550_A586_D719AAA0E213 */
