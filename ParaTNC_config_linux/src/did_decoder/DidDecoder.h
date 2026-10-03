/*
 * DidDecoder.h
 *
 *  Created on: Oct 3, 2026
 *      Author: mateusz
 */

#ifndef SRC_DID_DECODER_DIDDECODER_H_
#define SRC_DID_DECODER_DIDDECODER_H_

#include "DidDescription.h"
#include "types/DidResponse.h"

#include <map>

class DidDecoder {
	/**
	 * @brief map of DID description read from INI file
	 */
	const std::map<uint16_t, DidDescription> &m_descriptions;

	/**
	 * Decodes any decimal value from a DID
	 * @param value raw value returned from the controller
	 * @param descr description of this DID
	 */
	void printDidVariable (const DidResponse_Data &value, const DidDescriptionSingleVariable &descr,
				   DidResponse_DataSize type);

	/**
	 * Float values require some distinct formatting
	 * @param value raw value returned from the controller
	 * @param descr description of this DID
	 */
	void printDidVariable (float value, const DidDescriptionSingleVariable &descr);

  public:
	DidDecoder (const std::map<uint16_t, DidDescription> &descriptions);
	virtual ~DidDecoder ();

	/**
	 * Uses map with DID descriptions to pretty-print data returned by the controller
	 * @param didNumberId
	 * @param response received from the controller
	 * @return false if map doesn't have description for did with given, true otherwise
	 */
	bool decodeAndPrintDid (uint16_t didNumberId, const DidResponse &response);
};

#endif /* SRC_DID_DECODER_DIDDECODER_H_ */
