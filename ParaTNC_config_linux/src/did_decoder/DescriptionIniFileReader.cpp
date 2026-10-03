#include "DescriptionIniFileReader.h"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

extern bool verboseLogging;

namespace {

/**
 * @brief single key=value pair as read from INI file
 */
struct IniEntry {
	std::string value;
	bool quoted; //!< value was enclosed in double quotation marks
	int line;
};

/**
 * @brief single [section] with all its keys. Key names are lowercase
 */
struct IniSection {
	std::string name;
	int line;
	std::map<std::string, IniEntry> entries;
};

const char *const HEADER_SECTION = "header";

const char *const VARIABLE_PREFIXES[] = {"1st", "2nd", "3rd"};

const char *const VARIABLE_KEYS[] = {
	"scalinga", "scalingb", "scalingc", "scalingd", "name", "unit"};

std::string toLowerCase (const std::string &str)
{
	std::string result = str;
	std::transform (result.begin (), result.end (), result.begin (), [] (unsigned char c) {
		return std::tolower (c);
	});
	return result;
}

std::string trim (const std::string &str)
{
	size_t start = str.find_first_not_of (" \t\r\n\v\f");
	if (start == std::string::npos)
		return "";

	size_t end = str.find_last_not_of (" \t\r\n\v\f");
	return str.substr (start, (end - start + 1));
}

/**
 * @brief removes everything from '#' until the end of line, unless '#' is
 * placed between double quotation marks
 */
std::string stripComment (const std::string &line)
{
	bool inQuotes = false;

	for (size_t i = 0; i < line.length (); i++) {
		if (line[i] == '"') {
			inQuotes = !inQuotes;
		}
		else if (line[i] == '#' && !inQuotes) {
			return line.substr (0, i);
		}
	}

	return line;
}

/**
 * @brief removes all whitespaces from unquoted value
 */
std::string removeWhitespaces (const std::string &str)
{
	std::string result;
	for (unsigned char c : str) {
		if (!std::isspace (c)) {
			result += static_cast<char> (c);
		}
	}
	return result;
}

/**
 * @brief parses base 10 or base 16 (prefixed with '0x') integer, with optional minus sign
 * @return false if text is not a valid number or it is out of int64 range
 */
bool parseNumber (const std::string &text, int64_t &out)
{
	size_t pos = 0;
	bool negative = false;

	if (pos < text.length () && (text[pos] == '-' || text[pos] == '+')) {
		negative = (text[pos] == '-');
		pos++;
	}

	int base = 10;
	if (text.length () > pos + 1 && text[pos] == '0' &&
		(text[pos + 1] == 'x' || text[pos + 1] == 'X')) {
		base = 16;
		pos += 2;
	}

	const std::string digits = text.substr (pos);
	if (digits.empty ()) {
		return false;
	}

	for (unsigned char c : digits) {
		if ((base == 10 && !std::isdigit (c)) || (base == 16 && !std::isxdigit (c))) {
			return false;
		}
	}

	errno = 0;
	const unsigned long long magnitude = std::strtoull (digits.c_str (), nullptr, base);
	if (errno == ERANGE) {
		return false;
	}

	if (negative) {
		if (magnitude >
			static_cast<unsigned long long> (std::numeric_limits<int64_t>::max ()) + 1) {
			return false;
		}
		out = static_cast<int64_t> (0 - magnitude);
	}
	else {
		if (magnitude > static_cast<unsigned long long> (std::numeric_limits<int64_t>::max ())) {
			return false;
		}
		out = static_cast<int64_t> (magnitude);
	}

	return true;
}

/**
 * @brief parses DID number, either from section name or from DIDList
 * @return false if text is not a number or it doesn't fit into uint16_t
 */
bool parseDid (const std::string &text, uint16_t &out)
{
	int64_t value = 0;
	if (!parseNumber (text, value) || value < 0 || value > 0xFFFF) {
		return false;
	}
	out = static_cast<uint16_t> (value);
	return true;
}

/**
 * @brief Reads and validates everything from single INI file. Keeps a name of the
 * file to put it into exception message.
 */
class Parser {
  public:
	explicit Parser (const std::string &fileName) : m_fileName (fileName) {}

	/**
	 * @brief splits INI file content into sections and key-value pairs, checks syntax
	 */
	std::vector<IniSection> tokenize (std::istream &input);

	const IniEntry &getMandatory (const IniSection &section, const std::string &key);
	std::string getString (const IniSection &section, const std::string &key);
	int64_t getNumber (const IniSection &section, const std::string &key, int64_t min, int64_t max);

	DidDescription parseDidSection (const IniSection &section, uint16_t id);

	[[noreturn]] void fail (int line, const std::string &message) const
	{
		throw DescriptionIniParseError (m_fileName, line, message);
	}

  private:
	void parseSectionHeader (const std::string &line, int lineNumber, std::vector<IniSection> &out);
	void parseKeyValue (const std::string &line, int lineNumber, std::vector<IniSection> &out);

	const std::string &m_fileName;
};

std::vector<IniSection> Parser::tokenize (std::istream &input)
{
	std::vector<IniSection> sections;
	std::string rawLine;
	int lineNumber = 0;

	while (std::getline (input, rawLine)) {
		lineNumber++;

		const std::string line = trim (stripComment (rawLine));

		if (line.empty ()) {
			continue;
		}

		if (line[0] == '[') {
			parseSectionHeader (line, lineNumber, sections);
		}
		else {
			parseKeyValue (line, lineNumber, sections);
		}
	}

	if (input.bad ()) {
		fail (lineNumber, "I/O error while reading the file");
	}

	return sections;
}

void Parser::parseSectionHeader (const std::string &line, int lineNumber,
								 std::vector<IniSection> &out)
{
	if (line.back () != ']') {
		fail (lineNumber, "section header must be terminated with ']'");
	}

	const std::string name = toLowerCase (trim (line.substr (1, line.length () - 2)));

	if (name.empty ()) {
		fail (lineNumber, "empty section name");
	}

	if (name.find_first_of ("[]\"") != std::string::npos) {
		fail (lineNumber, "invalid character in section name '" + name + "'");
	}

	for (const IniSection &existing : out) {
		if (existing.name == name) {
			fail (lineNumber,
				  "duplicated section [" + name + "], first defined in line " +
					  std::to_string (existing.line));
		}
	}

	if (verboseLogging) {
		std::cout << "---- Parser::parseSectionHeader, lineNumber: " << lineNumber
				  << ", name: " << name << std::endl;
	}

	out.push_back (IniSection{name, lineNumber, {}});
}

void Parser::parseKeyValue (const std::string &line, int lineNumber, std::vector<IniSection> &out)
{
	const size_t equalPos = line.find ('=');

	if (equalPos == std::string::npos) {
		fail (lineNumber, "expected 'Key=Value' or '[Section]', got '" + line + "'");
	}

	const std::string key = toLowerCase (trim (line.substr (0, equalPos)));
	const std::string rawValue = trim (line.substr (equalPos + 1));

	if (key.empty ()) {
		fail (lineNumber, "missing key name before '='");
	}

	for (unsigned char c : key) {
		if (!std::isalnum (c) && c != '_') {
			fail (lineNumber, "invalid character in key name '" + key + "'");
		}
	}

	if (out.empty ()) {
		fail (lineNumber, "key '" + key + "' defined outside of any section");
	}

	IniEntry entry{"", false, lineNumber};

	if (!rawValue.empty () && rawValue[0] == '"') {
		const size_t closingPos = rawValue.find ('"', 1);
		if (closingPos == std::string::npos) {
			fail (lineNumber, "missing closing '\"' in value of key '" + key + "'");
		}
		if (closingPos != rawValue.length () - 1) {
			fail (lineNumber,
				  "unexpected characters after closing '\"' in value of key '" + key + "'");
		}
		entry.value = rawValue.substr (1, closingPos - 1);
		entry.quoted = true;
	}
	else {
		if (rawValue.find ('"') != std::string::npos) {
			fail (lineNumber, "misplaced '\"' in value of key '" + key + "'");
		}
		entry.value = removeWhitespaces (rawValue);
		if (entry.value.empty ()) {
			fail (lineNumber, "missing value for key '" + key + "'");
		}
	}

	IniSection &section = out.back ();
	const auto existing = section.entries.find (key);
	if (existing != section.entries.end ()) {
		fail (lineNumber,
			  "duplicated key '" + key + "' in section [" + section.name +
				  "], first defined in line " + std::to_string (existing->second.line));
	}

	if (verboseLogging) {
		std::cout << "---- Parser::parseKeyValue, lineNumber: " << lineNumber << ", key: " << key
				  << ", entry.value: " << entry.value << std::endl;
	}

	section.entries.emplace (key, entry);
}

const IniEntry &Parser::getMandatory (const IniSection &section, const std::string &key)
{
	const auto it = section.entries.find (key);
	if (it == section.entries.end ()) {
		fail (section.line,
			  "missing mandatory key '" + key + "' in section [" + section.name + "]");
	}
	return it->second;
}

std::string Parser::getString (const IniSection &section, const std::string &key)
{
	const IniEntry &entry = getMandatory (section, key);
	if (!entry.quoted) {
		fail (entry.line, "value of key '" + key + "' must be a string enclosed in '\"'");
	}
	return entry.value;
}

int64_t Parser::getNumber (const IniSection &section, const std::string &key, int64_t min,
						   int64_t max)
{
	const IniEntry &entry = getMandatory (section, key);
	int64_t value = 0;

	if (entry.quoted || !parseNumber (entry.value, value)) {
		fail (entry.line,
			  "value of key '" + key + "' must be a decimal or hexadecimal (0x) number");
	}

	if (value < min || value > max) {
		fail (entry.line,
			  "value of key '" + key + "' is out of range <" + std::to_string (min) + ", " +
				  std::to_string (max) + ">");
	}

	return value;
}

DidDescription Parser::parseDidSection (const IniSection &section, uint16_t id)
{
	DidDescription description;
	description.id = id;
	description.shortName = getString (section, "shortname");
	description.longerDescription = getString (section, "longerdescription");

	// check if there are no unknown (e.g. misspelled) keys in this section
	for (const auto &entry : section.entries) {
		const std::string &key = entry.first;
		bool known = (key == "shortname" || key == "longerdescription");

		for (const char *prefix : VARIABLE_PREFIXES) {
			for (const char *variableKey : VARIABLE_KEYS) {
				if (key == std::string (prefix) + variableKey) {
					known = true;
				}
			}
		}

		if (!known) {
			fail (entry.second.line, "unknown key '" + key + "' in section [" + section.name + "]");
		}
	}

	const int64_t int32Min = std::numeric_limits<int32_t>::min ();
	const int64_t int32Max = std::numeric_limits<int32_t>::max ();

	bool previousMissing = false;

	for (const char *prefix : VARIABLE_PREFIXES) {
		const std::string p (prefix);

		const bool anyPresent =
			std::any_of (std::begin (VARIABLE_KEYS), std::end (VARIABLE_KEYS), [&] (const char *k) {
				return section.entries.count (p + k) != 0;
			});

		if (!anyPresent) {
			previousMissing = true;
			continue;
		}

		if (previousMissing) {
			fail (section.line,
				  "variable '" + p + "' defined in section [" + section.name +
					  "] while previous one is missing");
		}

		// getMandatory throws if the set of keys for this variable is incomplete
		DidDescriptionSingleVariable variable;
		variable.scalingA =
			static_cast<int32_t> (getNumber (section, p + "scalinga", int32Min, int32Max));
		variable.scalingB =
			static_cast<int32_t> (getNumber (section, p + "scalingb", int32Min, int32Max));
		variable.scalingC =
			static_cast<int32_t> (getNumber (section, p + "scalingc", int32Min, int32Max));
		variable.scalingD =
			static_cast<int32_t> (getNumber (section, p + "scalingd", int32Min, int32Max));
		variable.name = getString (section, p + "name");
		variable.unit = getString (section, p + "unit");

		description.variables.push_back (variable);
	}

	if (description.variables.empty ()) {
		fail (section.line,
			  "section [" + section.name +
				  "] must define at least one variable (1stScalingA, 1stScalingB, "
				  "1stScalingC, 1stScalingD, 1stName, 1stUnit)");
	}

	return description;
}

} // namespace

DescriptionIniParseError::DescriptionIniParseError (const std::string &fileName, int line,
													const std::string &message)
	: std::runtime_error (fileName + ":" + std::to_string (line) + ": " + message), m_line (line)
{
}

DescriptionIniFileReader::DescriptionIniFileReader (std::string fileName) : m_fileName (fileName)
{
}

bool DescriptionIniFileReader::parse ()
{
	Parser parser (m_fileName);

	std::ifstream file (m_fileName);
	if (!file.is_open ()) {
		return false;
	}

	const std::vector<IniSection> sections = parser.tokenize (file);

	// --- [Header] section ---
	const auto header = std::find_if (sections.begin (), sections.end (), [] (const IniSection &s) {
		return s.name == HEADER_SECTION;
	});

	if (header == sections.end ()) {
		parser.fail (0, "missing mandatory section [Header]");
	}

	m_headerDescription = parser.getString (*header, "description");
	m_creationDate = parser.getString (*header, "creationdate");
	const std::string versionFrom = parser.getString (*header, "versionfrom");
	const std::string versionTo = parser.getString (*header, "versionto");

	for (const auto &entry : header->entries) {
		const std::string &key = entry.first;
		if (key != "description" && key != "creationdate" && key != "versionfrom" &&
			key != "versionto" && key != "didlist") {
			parser.fail (entry.second.line, "unknown key '" + key + "' in section [Header]");
		}
	}

	const IniEntry &didListEntry = parser.getMandatory (*header, "didlist");
	if (didListEntry.quoted) {
		parser.fail (didListEntry.line, "value of key 'didlist' must be a list of numbers");
	}

	std::vector<uint16_t> didList;
	std::istringstream didListStream (didListEntry.value);
	std::string item;
	while (std::getline (didListStream, item, ',')) {
		uint16_t did = 0;
		if (!parseDid (item, did)) {
			parser.fail (didListEntry.line,
						 "invalid DID '" + item +
							 "' in 'didlist', expected "
							 "a number in range 0 - 0xFFFF");
		}
		if (std::find (didList.begin (), didList.end (), did) != didList.end ()) {
			parser.fail (didListEntry.line,
						 "DID '" + item + "' listed more than once in 'didlist'");
		}
		didList.push_back (did);
	}

	if (didList.empty () || didListEntry.value.back () == ',') {
		parser.fail (didListEntry.line, "malformed or empty 'didlist'");
	}

	// --- DID sections ---
	std::map<uint16_t, DidDescription> descriptions;

	for (const IniSection &section : sections) {
		if (section.name == HEADER_SECTION) {
			continue;
		}

		uint16_t did = 0;
		if (!parseDid (section.name, did)) {
			parser.fail (section.line,
						 "section name [" + section.name +
							 "] is neither 'Header' nor a valid DID number");
		}

		if (std::find (didList.begin (), didList.end (), did) == didList.end ()) {
			parser.fail (section.line, "section [" + section.name + "] is not listed in 'didlist'");
		}

		if (descriptions.count (did) != 0) {
			parser.fail (section.line, "DID [" + section.name + "] is defined more than once");
		}

		descriptions.emplace (did, parser.parseDidSection (section, did));
	}

	for (uint16_t did : didList) {
		if (descriptions.count (did) == 0) {
			std::ostringstream hex;
			hex << "0x" << std::hex << std::uppercase << did;
			parser.fail (didListEntry.line,
						 "DID " + hex.str () +
							 " is listed in 'didlist' but its section is missing");
		}
	}

	// everything is OK, commit parsed data
	m_versionFrom = versionFrom;
	m_versionTo = versionTo;
	m_Descriptions = std::move (descriptions);

	return true;
}

bool DescriptionIniFileReader::hasDescriptionForDid (uint16_t did)
{
	bool out = false;

	std::map<uint16_t, DidDescription>::const_iterator it = m_Descriptions.find (did);

	if (it != m_Descriptions.end ()) {
		out = true;
	}

	if (verboseLogging) {
		std::cout << "---- DescriptionIniFileReader::hasDescriptionForDid, did: 0x" << std::hex
				  << (int)did << std::dec << ", out: " << (int)out << std::endl;
	}

	return out;
}
