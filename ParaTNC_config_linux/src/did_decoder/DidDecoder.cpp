/*
 * DidDecoder.cpp
 *
 *  Created on: Oct 3, 2026
 *      Author: mateusz
 */

#include <did_decoder/DidDecoder.h>

#include <iostream>
#include <stdexcept>

DidDecoder::DidDecoder (const std::map<uint16_t, DidDescription> &descriptions)
	: m_descriptions (descriptions)
{
}

DidDecoder::~DidDecoder ()
{
}

void DidDecoder::printDidVariable (const DidResponse_Data &value, const DidDescriptionSingleVariable &descr,
						   DidResponse_DataSize type)
{
	// local variable to store value to be printed
	int32_t v = 0;

	// depending on a type, the value is stored in different union field
	switch (type) {
		case DIDRESPONSE_DATASIZE_INT8:		v = value.i8; break;
		case DIDRESPONSE_DATASIZE_INT16:	v = value.i16; break;
		case DIDRESPONSE_DATASIZE_INT32:	v = value.i32; break;
		default: {
			std::cout << "E = DidDecoder::printDid , DID should be an integer type at this place!" << std::endl;
			throw std::runtime_error ("We should'nt be there!");
		}
	}

	float decoded = (descr.scalingA * v * v) + descr.scalingB * v + descr.scalingC;

	std::cout << "I = DidDecoder::printDidVariable, \t" << descr.name << " - ";

	// Division by zero if of course not allowed. If this scalling coeff is set to zero
	// simply print raw value as-is
	if (descr.scalingD == 0)
	{
		std::cout << "0x" << std::hex << v << std::dec << " " << descr.unit << std::endl;
	}
	// check if result are integer or float
	else if (descr.scalingD == 1)
	{
		decoded /= descr.scalingD;
		// for sure it is decimal.
		std::cout << (int32_t)decoded << " " << descr.unit << std::endl;
	}
	else
	{
		decoded /= descr.scalingD;
		// it might be decimal, but print it as float
		std::cout << decoded << " " << descr.unit << std::endl;
	}
}

void DidDecoder::printDidVariable (float value, const DidDescriptionSingleVariable &descr)
{
	float decoded = (descr.scalingA * value * value) + descr.scalingB * value + descr.scalingC;
	decoded /= descr.scalingD;

	std::cout << "I = DidDecoder::printDidVariable, \t" << descr.name << " - ";

	// it might be decimal, but print it as float
	std::cout << decoded << " " << descr.unit << std::endl;

}

bool DidDecoder::decodeAndPrintDid (uint16_t didNumberId, const DidResponse &response)
{
	bool out = true;

	// look for a description of DID with given id number
	std::map<uint16_t, DidDescription>::const_iterator value = m_descriptions.find (didNumberId);

	if (value == m_descriptions.end ()) {
		out = false;
	}
	else {
		const DidDescription &description = value->second;

		int howMany = 0; // variables are returned by this DID

		if (response.firstSize != DIDRESPONSE_DATASIZE_EMPTY) {
			howMany++;
		}

		if (response.secondSize != DIDRESPONSE_DATASIZE_EMPTY) {
			howMany++;
		}

		if (response.thirdSize != DIDRESPONSE_DATASIZE_EMPTY) {
			howMany++;
		}

		if (howMany == 0) {
			std::cout << "E = DidDecoder::printDid , didNumberId: " << didNumberId <<  std::endl;
			throw std::runtime_error ("It doesn't make any sense for DID to be empty!");
		}
		else {
			// print description for this DID
			std::cout << "I = DidDecoder::decodeAndPrintDid, 0x" << std::hex << didNumberId
					  << std::dec << " - " << description.shortName << " - "
					  << description.longerDescription << std::endl;
		}

		if ((response.firstSize == DIDRESPONSE_DATASIZE_STRING)) {
			// if the DID returns a string, it may contain only single one.
			// in such cases there is of course no recalculation or decoding, and
			// the string is printed as-is. DidDescription is used only to
			// print a name and description of this DID, to give some more context
			std::cout << "I = DidDecoder::decodeAndPrintDid, text: " << response.first.str;
		}
		else {
			if (response.firstSize == DIDRESPONSE_DATASIZE_EMPTY) {
				throw std::runtime_error (
					"first variable in a DID can't be empty, as this doesn't make "
					"sense for the DID to be empty.");
			}

			// description INI file must define all numeric variables returned by this DID
			if (howMany != (int)description.variables.size ()) {
				throw std::runtime_error ("description INI file must contain definitions for all "
										  "variables in single DID");
			}

			// print first DID
			if (howMany > 0) {
				if (response.firstSize == DIDRESPONSE_DATASIZE_FLOAT) {
					printDidVariable (response.first.f, description.variables.at(0));
				}
				else {
					printDidVariable (response.first, description.variables.at(0), response.firstSize);
				}
			}

			// print second DID
			if (howMany > 1) {
				if (response.secondSize == DIDRESPONSE_DATASIZE_FLOAT) {
					printDidVariable (response.second.f, description.variables.at(1));
				}
				else {
					printDidVariable (response.second, description.variables.at(1), response.secondSize);
				}
			}

			// print third DID
			if (howMany > 2) {
				if (response.thirdSize == DIDRESPONSE_DATASIZE_FLOAT) {
					printDidVariable (response.third.f, description.variables.at(2));
				}
				else {
					printDidVariable (response.third, description.variables.at(2), response.thirdSize);
				}
			}
		}
	}
	return out;
}
