#ifndef B09C8AC9_21CF_4A64_9337_2BE1A5F65837
#define B09C8AC9_21CF_4A64_9337_2BE1A5F65837

#include "DidDescription.h"
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Thrown by DescriptionIniFileReader::parse when INI file is malformed
 * or doesn't contain all mandatory data. what() returns a message in format
 * "<file>:<line>: <description of an error>"
 */
class DescriptionIniParseError : public std::runtime_error {
  public:
	DescriptionIniParseError (const std::string &fileName, int line, const std::string &message);

	/**
	 * @brief line number (counting from 1) of INI file where an error was detected,
	 * or 0 if an error is not related to any specific line (like file cannot be opened)
	 */
	int getLine () const { return m_line; }

  private:
	int m_line;
};

/**
 * @brief Decode an INI file containing descriptions of DIDs
 *
 * The INI file has structure and syntax of standard Windows INI files. Symbol '#' starts a comment.
 * Everything starting from '#' until the end of line is ignored. Key values are case insensitive,
 * i.e. "name", "Name" and "NAME" is the same key. Values *are* case sensitive and must be
 * escaped by double quotation mark if they contain a string. Non-printable characters like spaces
 * tabs etc. are ignored and skipped, if they're not escaped using double quotation marks.
 * Because of that "Key = Value" and "Key=Value" and "Key		=	Value" have the same effect.
 * Numbers are base 10 by default, they may be base 16 if prefixed with '0x'. Value may contain
 * a list of numbers, each member of a list separated by ','
 *
 * File contains exact one section working as a header with basic information of the content.
 * See snippet below. All of those keys are mandatory and parsing shall stop with an error
 * if anything is missing.
 *
 * [Header]
 * Description="Printable description of this file, like when it was created etc."
 * CreationDate=28092026
 * VersionFrom="FA06"
 * VersionTo="FB00"
 * DIDList=0x1000,0x1001,0x1002,0x2100
 *
 * A list 'DIDList' contains identifiers of all other sections stored in the same INI file.
 * In this case the file will contain sections: [Header] [0x1000] [0x1001] [0x1002] [0x2100]
 *
 * Single DID can return one, two or three variables. A section for single DID must contain keys
 * 	- ShortName
 * 	- LongerDescription
 *
 * 	and at least one set of keys
 * 	 - xxxScalingA
 * 	 - xxxScalingB
 * 	 - xxxScalingC
 * 	 - xxxScalingD
 * 	 - xxxName
 * 	 - xxxUnit
 * 	where 'xxx' can be: 1st or 2nd or 3rd.
 *
 * If DID section doesn't contain 'ShortName' or 'LongerDescriuption' key, parsing should be
 * stopped with an error. Each variable within one DID section must contain complete set of keys.
 * For instance, if 1stScalingD is missing, parsing should be stopped.
 *
 * Example section for single DID looks like that. Everything from this single section
 * goes into single instance of @link{DidDescription}. DidDescription::id is set to value
 * from section name. In case of this example -> 0x1000
 *
 * [0x1000]
 * ShortName="Uptime"
 * LongerDescription="Amount of seconds from power on or restart"
 * 1stScalingA=0			# parsed into single instance of DidDescriptionSingleVariable
 * 1stScalingB=1			# parsed into single instance of DidDescriptionSingleVariable
 * 1stScalingC=0			# parsed into single instance of DidDescriptionSingleVariable
 * 1stScalingD=1			# parsed into single instance of DidDescriptionSingleVariable
 * 1stName="uptime"			# parsed into single instance of DidDescriptionSingleVariable
 * 1stUnit="seconds"		# parsed into single instance of DidDescriptionSingleVariable
 *
 *
 */
class DescriptionIniFileReader {
  public:
	DescriptionIniFileReader (std::string fileName);

	/**
	 * @brief parse INI file with a structure described in docstring of this class,
	 * and puts data into @link{m_Descriptions} map
	 * @return true if file was parsed successfully
	 * @throw DescriptionIniParseError if file cannot be read, is malformed or any
	 * mandatory data is missing. Class members are not modified in such case.
	 */
	bool parse ();

	bool hasDescriptionForDid (uint16_t did);

	/**
	 * @brief returns identifiers of all DIDs stored in @link{m_Descriptions}
	 * @return vector of DIDs, sorted ascending (in the order of map keys)
	 */
	std::vector<uint16_t> getDidList () const;

	const std::string &getVersionFrom () const { return m_versionFrom; }
	const std::string &getVersionTo () const { return m_versionTo; }
	const std::map<uint16_t, DidDescription> &getDescriptions () const { return m_Descriptions; }
	const std::string &getCreationDate () const { return m_creationDate; }
	const std::string &getHeaderDescription () const { return m_headerDescription; }

  private:
	/**
	 * @brief path and filename of INI file with DID Description
	 */
	std::string m_fileName;

	/**
	 * @brief the lowest version of controller software this description is valid for
	 */
	std::string m_versionFrom;

	/**
	 * @brief the highest version of controller software this description is valid for
	 */
	std::string m_versionTo;

	std::string m_creationDate;

	std::string m_headerDescription;

	/**
	 * @brief all DIDs read from INI file.
	 * @note Key data identifier, basically a value of 'id' in DidDescription class
	 * @note Value description
	 */
	std::map<uint16_t, DidDescription> m_Descriptions;
};

#endif /* B09C8AC9_21CF_4A64_9337_2BE1A5F65837 */
