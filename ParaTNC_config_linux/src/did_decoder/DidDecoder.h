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
	 * @brief Name of a log file to which this class will output decoded DID values
	 * @note logging will be disabled if the string has a length of 0 or 1
	 */
	const std::string m_logFilename;

	bool m_isLogFilename;

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
	DidDecoder (const std::map<uint16_t, DidDescription> &descriptions, std::string logFileName);
	virtual ~DidDecoder ();

	/**
	 * Uses map with DID descriptions to pretty-print data returned by the controller
	 * @param didNumberId
	 * @param response received from the controller
	 * @return false if map doesn't have description for did with given, true otherwise
	 */
	bool decodeAndPrintDid (uint16_t didNumberId, const DidResponse &response);

	/**
	 * Is set to true member function @link{decodeAndPrint} will put a DID name and
	 * DID description on the console
	 */
	bool m_printDidNameDescription;
};

#endif /* SRC_DID_DECODER_DIDDECODER_H_ */
