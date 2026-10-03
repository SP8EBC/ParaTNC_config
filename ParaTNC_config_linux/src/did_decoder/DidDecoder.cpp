/*
 * DidDecoder.cpp
 *
 *  Created on: Oct 3, 2026
 *      Author: mateusz
 */

#include <did_decoder/DidDecoder.h>

DidDecoder::DidDecoder (const std::map<uint16_t, DidDescription> &descriptions)
	: m_descriptions (descriptions)
{
}

DidDecoder::~DidDecoder ()
{
	// TODO Auto-generated destructor stub
}

bool DidDecoder::decodeAndPrintDid (uint16_t didNumberId, DidResponse &response)
{
	bool out = false;

	// look for a description of DID with given id number
	std::map<uint16_t, DidDescription>::const_iterator value = m_descriptions.find(didNumberId);

	if (value != m_descriptions.end())
	{
		out = true;
	}
	else
	{
		;	// value not found
	}

	return out;
}
