/*
 * DescriptionIniFileReader_test.cpp
 *
 * Unit tests for DescriptionIniFileReader. Build and run using Makefile
 * located in the same directory: 'make test'
 */

#include "DescriptionIniFileReader.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DESCRIPTIONINIFILEREADER
#include <boost/test/unit_test.hpp>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace {

/**
 * @brief Minimal, valid file used as a base for most of test cases. Line numbers
 * are important, as many tests modify single line and check if the error is reported
 * for exactly this line.
 */
const char *const VALID_INI = "[Header]\n"										// 1
							  "Description=\"Test file\"\n"						// 2
							  "CreationDate=28092026\n"							// 3
							  "VersionFrom=\"FA06\"\n"							// 4
							  "VersionTo=\"FB00\"\n"							// 5
							  "DIDList=0x1000\n"								// 6
							  "\n"												// 7
							  "[0x1000]\n"										// 8
							  "ShortName=\"Uptime\"\n"							// 9
							  "LongerDescription=\"Seconds from power on\"\n"	// 10
							  "1stScalingA=0\n"									// 11
							  "1stScalingB=1\n"									// 12
							  "1stScalingC=0\n"									// 13
							  "1stScalingD=1\n"									// 14
							  "1stName=\"uptime\"\n"							// 15
							  "1stUnit=\"seconds\"\n";							// 16

/**
 * @brief Writes given content into temporary file, which is removed
 * when an instance goes out of scope
 */
struct TempIniFile {
	explicit TempIniFile (const std::string &content)
	{
		static int counter = 0;
		path = (std::filesystem::temp_directory_path () /
				("did_description_ut_" + std::to_string (counter++) + ".ini"))
				   .string ();
		write (content);
	}

	~TempIniFile () { std::remove (path.c_str ()); }

	void write (const std::string &content)
	{
		std::ofstream file (path, std::ios::trunc);
		file << content;
	}

	std::string path;
};

/**
 * @brief Replaces whole line number 'lineNumber' (counting from 1) in 'content'
 */
std::string replaceLine (const std::string &content, int lineNumber, const std::string &newLine)
{
	std::string result;
	std::istringstream input (content);
	std::string line;
	int current = 0;

	while (std::getline (input, line)) {
		current++;
		result += (current == lineNumber ? newLine : line) + "\n";
	}

	return result;
}

/**
 * @brief Parses content and checks that DescriptionIniParseError is thrown
 * for expected line and its message contains expected text
 */
void checkError (const std::string &content, int expectedLine, const std::string &expectedText)
{
	TempIniFile file (content);
	DescriptionIniFileReader reader (file.path);

	try {
		reader.parse ();
		BOOST_ERROR ("exception not thrown, expected: " << expectedText);
	}
	catch (const DescriptionIniParseError &e) {
		const std::string message (e.what ());
		BOOST_TEST_INFO ("message: " << message);
		BOOST_CHECK_EQUAL (e.getLine (), expectedLine);
		BOOST_CHECK (message.find (file.path + ":" + std::to_string (expectedLine) + ": ") == 0);
		BOOST_CHECK (message.find (expectedText) != std::string::npos);
	}

	BOOST_CHECK (reader.getDescriptions ().empty ());
}

} // namespace

BOOST_AUTO_TEST_SUITE (valid_files)

BOOST_AUTO_TEST_CASE (minimal_file)
{
	TempIniFile file (VALID_INI);
	DescriptionIniFileReader reader (file.path);

	BOOST_CHECK (reader.parse ());
	BOOST_CHECK_EQUAL (reader.getVersionFrom (), "FA06");
	BOOST_CHECK_EQUAL (reader.getVersionTo (), "FB00");
	BOOST_REQUIRE_EQUAL (reader.getDescriptions ().size (), 1u);

	const DidDescription &did = reader.getDescriptions ().at (0x1000);
	BOOST_CHECK_EQUAL (did.id, 0x1000);
	BOOST_CHECK_EQUAL (did.shortName, "Uptime");
	BOOST_CHECK_EQUAL (did.longerDescription, "Seconds from power on");
	BOOST_REQUIRE_EQUAL (did.variables.size (), 1u);
	BOOST_CHECK_EQUAL (did.variables[0].scalingA, 0);
	BOOST_CHECK_EQUAL (did.variables[0].scalingB, 1);
	BOOST_CHECK_EQUAL (did.variables[0].scalingC, 0);
	BOOST_CHECK_EQUAL (did.variables[0].scalingD, 1);
	BOOST_CHECK_EQUAL (did.variables[0].name, "uptime");
	BOOST_CHECK_EQUAL (did.variables[0].unit, "seconds");
}

BOOST_AUTO_TEST_CASE (example_from_docstring_with_comments)
{
	TempIniFile file ("[Header]\n"
					  "Description=\"Printable description of this file, like when it was "
					  "created etc.\"\n"
					  "CreationDate=28092026\n"
					  "VersionFrom=\"FA06\"\n"
					  "VersionTo=\"FB00\"\n"
					  "DIDList=0x1000\n"
					  "[0x1000]\n"
					  "ShortName=\"Uptime\"\n"
					  "LongerDescription=\"Amount of seconds from power on or restart\"\n"
					  "1stScalingA=0\t\t\t# parsed into single instance\n"
					  "1stScalingB=1\t\t\t# parsed into single instance\n"
					  "1stScalingC=0\t\t\t# parsed into single instance\n"
					  "1stScalingD=1\t\t\t# parsed into single instance\n"
					  "1stName=\"uptime\"\t\t\t# parsed into single instance\n"
					  "1stUnit=\"seconds\"\t\t# parsed into single instance\n");
	DescriptionIniFileReader reader (file.path);

	BOOST_CHECK (reader.parse ());
	BOOST_CHECK_EQUAL (reader.getDescriptions ().at (0x1000).variables.at (0).unit, "seconds");
}

BOOST_AUTO_TEST_CASE (three_dids_with_multiple_variables)
{
	TempIniFile file ("# leading comment\n"
					  "[Header]\n"
					  "Description=\"multi\"\n"
					  "CreationDate=1\n"
					  "VersionFrom=\"A\"\n"
					  "VersionTo=\"B\"\n"
					  "DIDList=0x1000, 0x1001 ,4098\n"
					  "[0x1000]\n"
					  "ShortName=\"one\"\n"
					  "LongerDescription=\"one variable\"\n"
					  "1stScalingA=1\n1stScalingB=2\n1stScalingC=3\n1stScalingD=4\n"
					  "1stName=\"a\"\n1stUnit=\"u\"\n"
					  "[0x1001]\n"
					  "ShortName=\"two\"\n"
					  "LongerDescription=\"two variables\"\n"
					  "1stScalingA=0\n1stScalingB=1\n1stScalingC=0\n1stScalingD=1\n"
					  "1stName=\"a\"\n1stUnit=\"u\"\n"
					  "2ndScalingA=5\n2ndScalingB=6\n2ndScalingC=7\n2ndScalingD=8\n"
					  "2ndName=\"b\"\n2ndUnit=\"v\"\n"
					  "[4098]\n"
					  "ShortName=\"three\"\n"
					  "LongerDescription=\"three variables\"\n"
					  "1stScalingA=0\n1stScalingB=1\n1stScalingC=0\n1stScalingD=1\n"
					  "1stName=\"a\"\n1stUnit=\"u\"\n"
					  "2ndScalingA=0\n2ndScalingB=1\n2ndScalingC=0\n2ndScalingD=1\n"
					  "2ndName=\"b\"\n2ndUnit=\"v\"\n"
					  "3rdScalingA=9\n3rdScalingB=10\n3rdScalingC=11\n3rdScalingD=12\n"
					  "3rdName=\"c\"\n3rdUnit=\"w\"\n");
	DescriptionIniFileReader reader (file.path);

	BOOST_CHECK (reader.parse ());

	const auto &descriptions = reader.getDescriptions ();
	BOOST_REQUIRE_EQUAL (descriptions.size (), 3u);
	BOOST_CHECK_EQUAL (descriptions.at (0x1000).variables.size (), 1u);
	BOOST_CHECK_EQUAL (descriptions.at (0x1001).variables.size (), 2u);
	BOOST_CHECK_EQUAL (descriptions.at (0x1002).variables.size (), 3u);
	BOOST_CHECK_EQUAL (descriptions.at (0x1002).id, 0x1002);

	const DidDescriptionSingleVariable &second = descriptions.at (0x1001).variables.at (1);
	BOOST_CHECK_EQUAL (second.scalingA, 5);
	BOOST_CHECK_EQUAL (second.scalingB, 6);
	BOOST_CHECK_EQUAL (second.scalingC, 7);
	BOOST_CHECK_EQUAL (second.scalingD, 8);
	BOOST_CHECK_EQUAL (second.name, "b");
	BOOST_CHECK_EQUAL (second.unit, "v");

	const DidDescriptionSingleVariable &third = descriptions.at (0x1002).variables.at (2);
	BOOST_CHECK_EQUAL (third.scalingA, 9);
	BOOST_CHECK_EQUAL (third.scalingD, 12);
	BOOST_CHECK_EQUAL (third.name, "c");
}

BOOST_AUTO_TEST_CASE (keys_and_sections_are_case_insensitive)
{
	std::string content = VALID_INI;
	content = replaceLine (content, 1, "[HEADER]");
	content = replaceLine (content, 4, "versionfrom=\"FA06\"");
	content = replaceLine (content, 5, "VERSIONTO=\"FB00\"");
	content = replaceLine (content, 8, "[0X1000]");
	content = replaceLine (content, 9, "shortNAME=\"Uptime\"");
	TempIniFile file (content);
	DescriptionIniFileReader reader (file.path);

	BOOST_CHECK (reader.parse ());
	BOOST_CHECK_EQUAL (reader.getDescriptions ().at (0x1000).shortName, "Uptime");
}

BOOST_AUTO_TEST_CASE (values_are_case_sensitive)
{
	TempIniFile file (replaceLine (VALID_INI, 4, "VersionFrom=\"fa06\""));
	DescriptionIniFileReader reader (file.path);

	BOOST_CHECK (reader.parse ());
	BOOST_CHECK_EQUAL (reader.getVersionFrom (), "fa06");
}

BOOST_AUTO_TEST_CASE (whitespaces_are_ignored_outside_quotes)
{
	std::string content = VALID_INI;
	content = replaceLine (content, 1, "  [ Header ]  ");
	content = replaceLine (content, 9, "ShortName\t\t=\t\"  Up time  \"   ");
	content = replaceLine (content, 12, "   1stScalingB   =   1   ");
	content = replaceLine (content, 13, "1stScalingC\t=\t0\r");
	TempIniFile file (content);
	DescriptionIniFileReader reader (file.path);

	BOOST_CHECK (reader.parse ());
	BOOST_CHECK_EQUAL (reader.getDescriptions ().at (0x1000).shortName, "  Up time  ");
	BOOST_CHECK_EQUAL (reader.getDescriptions ().at (0x1000).variables[0].scalingB, 1);
}

BOOST_AUTO_TEST_CASE (hash_inside_quotes_is_not_a_comment)
{
	TempIniFile file (replaceLine (VALID_INI, 10, "LongerDescription=\"value # 1\" # comment"));
	DescriptionIniFileReader reader (file.path);

	BOOST_CHECK (reader.parse ());
	BOOST_CHECK_EQUAL (reader.getDescriptions ().at (0x1000).longerDescription, "value # 1");
}

BOOST_AUTO_TEST_CASE (hexadecimal_and_negative_numbers)
{
	std::string content = VALID_INI;
	content = replaceLine (content, 11, "1stScalingA=-5");
	content = replaceLine (content, 12, "1stScalingB=0x7FFFFFFF");
	content = replaceLine (content, 13, "1stScalingC=-2147483648");
	content = replaceLine (content, 14, "1stScalingD=0xa");
	TempIniFile file (content);
	DescriptionIniFileReader reader (file.path);

	BOOST_CHECK (reader.parse ());
	const DidDescriptionSingleVariable &v = reader.getDescriptions ().at (0x1000).variables[0];
	BOOST_CHECK_EQUAL (v.scalingA, -5);
	BOOST_CHECK_EQUAL (v.scalingB, 2147483647);
	BOOST_CHECK_EQUAL (v.scalingC, -2147483648LL);
	BOOST_CHECK_EQUAL (v.scalingD, 10);
}

BOOST_AUTO_TEST_CASE (failed_parse_keeps_previous_data)
{
	TempIniFile file (VALID_INI);
	DescriptionIniFileReader reader (file.path);
	BOOST_REQUIRE (reader.parse ());

	file.write (replaceLine (VALID_INI, 14, "1stScalingD=0"));
	BOOST_CHECK_THROW (reader.parse (), DescriptionIniParseError);

	BOOST_CHECK_EQUAL (reader.getVersionFrom (), "FA06");
	BOOST_CHECK_EQUAL (reader.getDescriptions ().size (), 1u);
	BOOST_CHECK_EQUAL (reader.getDescriptions ().at (0x1000).variables[0].scalingD, 1);
}

BOOST_AUTO_TEST_SUITE_END ()

BOOST_AUTO_TEST_SUITE (syntax_errors)

BOOST_AUTO_TEST_CASE (file_does_not_exist)
{
	DescriptionIniFileReader reader ("/nonexistent/directory/file.ini");

	try {
		reader.parse ();
		BOOST_ERROR ("exception not thrown");
	}
	catch (const DescriptionIniParseError &e) {
		BOOST_CHECK_EQUAL (e.getLine (), 0);
		BOOST_CHECK (std::string (e.what ()).find ("cannot open") != std::string::npos);
	}
}

BOOST_AUTO_TEST_CASE (unterminated_section_header)
{
	checkError (replaceLine (VALID_INI, 8, "[0x1000"), 8, "terminated with ']'");
}

BOOST_AUTO_TEST_CASE (empty_section_name)
{
	checkError (replaceLine (VALID_INI, 8, "[  ]"), 8, "empty section name");
}

BOOST_AUTO_TEST_CASE (line_without_equal_sign)
{
	checkError (replaceLine (VALID_INI, 12, "1stScalingB 1"), 12, "expected 'Key=Value'");
}

BOOST_AUTO_TEST_CASE (missing_key_name)
{
	checkError (replaceLine (VALID_INI, 12, "=1"), 12, "missing key name");
}

BOOST_AUTO_TEST_CASE (invalid_character_in_key)
{
	checkError (replaceLine (VALID_INI, 12, "1st ScalingB=1"), 12, "invalid character");
}

BOOST_AUTO_TEST_CASE (missing_value)
{
	checkError (replaceLine (VALID_INI, 12, "1stScalingB=   # nothing"), 12, "missing value");
}

BOOST_AUTO_TEST_CASE (key_outside_of_section)
{
	checkError (std::string ("Orphan=1\n") + VALID_INI, 1, "outside of any section");
}

BOOST_AUTO_TEST_CASE (unterminated_quote)
{
	checkError (replaceLine (VALID_INI, 9, "ShortName=\"Uptime"), 9, "missing closing");
}

BOOST_AUTO_TEST_CASE (characters_after_closing_quote)
{
	checkError (replaceLine (VALID_INI, 9, "ShortName=\"Up\"time"), 9, "after closing");
}

BOOST_AUTO_TEST_CASE (misplaced_quote)
{
	checkError (replaceLine (VALID_INI, 9, "ShortName=Up\"time\""), 9, "misplaced");
}

BOOST_AUTO_TEST_CASE (duplicated_key_with_different_case)
{
	checkError (replaceLine (VALID_INI, 7, "versionFROM=\"FA07\""), 7, "duplicated key");
}

BOOST_AUTO_TEST_CASE (duplicated_section)
{
	checkError (std::string (VALID_INI) + "[0x1000]\n", 17, "duplicated section");
}

BOOST_AUTO_TEST_SUITE_END ()

BOOST_AUTO_TEST_SUITE (header_errors)

BOOST_AUTO_TEST_CASE (missing_header_section)
{
	std::string content = VALID_INI;
	content = content.substr (content.find ("[0x1000]"));
	checkError (content, 0, "missing mandatory section [Header]");
}

BOOST_AUTO_TEST_CASE (missing_mandatory_keys)
{
	checkError (replaceLine (VALID_INI, 2, ""), 1, "'description'");
	checkError (replaceLine (VALID_INI, 3, ""), 1, "'creationdate'");
	checkError (replaceLine (VALID_INI, 4, ""), 1, "'versionfrom'");
	checkError (replaceLine (VALID_INI, 5, ""), 1, "'versionto'");
	checkError (replaceLine (VALID_INI, 6, ""), 1, "'didlist'");
}

BOOST_AUTO_TEST_CASE (string_value_without_quotes)
{
	checkError (replaceLine (VALID_INI, 4, "VersionFrom=FA06"), 4, "must be a string");
}

BOOST_AUTO_TEST_CASE (creation_date_is_not_a_number)
{
	checkError (replaceLine (VALID_INI, 3, "CreationDate=\"28092026\""), 3, "must be a decimal");
	checkError (replaceLine (VALID_INI, 3, "CreationDate=28.09.2026"), 3, "must be a decimal");
}

BOOST_AUTO_TEST_CASE (unknown_key_in_header)
{
	checkError (replaceLine (VALID_INI, 7, "Author=\"me\""), 7, "unknown key 'author'");
}

BOOST_AUTO_TEST_CASE (malformed_did_list)
{
	checkError (replaceLine (VALID_INI, 6, "DIDList=0x1000,abc"), 6, "invalid DID 'abc'");
	checkError (replaceLine (VALID_INI, 6, "DIDList=0x1000,0x10000"), 6, "invalid DID");
	checkError (replaceLine (VALID_INI, 6, "DIDList=0x1000,,0x1001"), 6, "invalid DID");
	checkError (replaceLine (VALID_INI, 6, "DIDList=0x1000,"), 6, "malformed");
	checkError (replaceLine (VALID_INI, 6, "DIDList=\"0x1000\""), 6, "list of numbers");
	checkError (replaceLine (VALID_INI, 6, "DIDList=0x1000,4096"), 6, "more than once");
}

BOOST_AUTO_TEST_CASE (did_listed_but_section_missing)
{
	checkError (replaceLine (VALID_INI, 6, "DIDList=0x1000,0x2100"), 6,
				"DID 0x2100 is listed in 'didlist' but its section is missing");
}

BOOST_AUTO_TEST_SUITE_END ()

BOOST_AUTO_TEST_SUITE (did_section_errors)

BOOST_AUTO_TEST_CASE (section_not_listed_in_did_list)
{
	checkError (replaceLine (VALID_INI, 8, "[0x1001]"), 8, "not listed in 'didlist'");
}

BOOST_AUTO_TEST_CASE (section_name_is_not_a_did)
{
	checkError (replaceLine (VALID_INI, 8, "[Uptime]"), 8, "neither 'Header' nor a valid DID");
	checkError (replaceLine (VALID_INI, 8, "[0x10000]"), 8, "neither 'Header' nor a valid DID");
}

BOOST_AUTO_TEST_CASE (same_did_written_differently)
{
	// [4096] is the same DID as [0x1000]
	checkError (std::string (VALID_INI) + "[4096]\n", 17, "defined more than once");
}

BOOST_AUTO_TEST_CASE (missing_short_name_or_description)
{
	checkError (replaceLine (VALID_INI, 9, ""), 8, "'shortname'");
	checkError (replaceLine (VALID_INI, 10, ""), 8, "'longerdescription'");
}

BOOST_AUTO_TEST_CASE (incomplete_variable)
{
	checkError (replaceLine (VALID_INI, 11, ""), 8, "'1stscalinga'");
	checkError (replaceLine (VALID_INI, 14, ""), 8, "'1stscalingd'");
	checkError (replaceLine (VALID_INI, 15, ""), 8, "'1stname'");
	checkError (replaceLine (VALID_INI, 16, ""), 8, "'1stunit'");
	checkError (std::string (VALID_INI) + "2ndScalingA=0\n", 8, "'2ndscalingb'");
}

BOOST_AUTO_TEST_CASE (no_variables)
{
	std::string content = VALID_INI;
	for (int line = 11; line <= 16; line++) {
		content = replaceLine (content, line, "");
	}
	checkError (content, 8, "at least one variable");
}

BOOST_AUTO_TEST_CASE (second_variable_without_first)
{
	std::string content = VALID_INI;
	content = replaceLine (content, 11, "2ndScalingA=0");
	content = replaceLine (content, 12, "2ndScalingB=1");
	content = replaceLine (content, 13, "2ndScalingC=0");
	content = replaceLine (content, 14, "2ndScalingD=1");
	content = replaceLine (content, 15, "2ndName=\"uptime\"");
	content = replaceLine (content, 16, "2ndUnit=\"seconds\"");
	checkError (content, 8, "previous one is missing");
}

BOOST_AUTO_TEST_CASE (unknown_key_in_did_section)
{
	checkError (replaceLine (VALID_INI, 12, "1stScallingB=1"), 12, "unknown key '1stscallingb'");
	checkError (std::string (VALID_INI) + "4thName=\"x\"\n", 17, "unknown key '4thname'");
}

BOOST_AUTO_TEST_CASE (wrong_value_types)
{
	checkError (replaceLine (VALID_INI, 12, "1stScalingB=\"1\""), 12, "must be a decimal");
	checkError (replaceLine (VALID_INI, 12, "1stScalingB=1.5"), 12, "must be a decimal");
	checkError (replaceLine (VALID_INI, 12, "1stScalingB=0x"), 12, "must be a decimal");
	checkError (replaceLine (VALID_INI, 12, "1stScalingB=0xG1"), 12, "must be a decimal");
	checkError (replaceLine (VALID_INI, 15, "1stName=uptime"), 15, "must be a string");
}

BOOST_AUTO_TEST_CASE (scaling_out_of_int32_range)
{
	checkError (replaceLine (VALID_INI, 13, "1stScalingC=2147483648"), 13, "out of range");
	checkError (replaceLine (VALID_INI, 13, "1stScalingC=-2147483649"), 13, "out of range");
	checkError (replaceLine (VALID_INI, 13, "1stScalingC=0x100000000"), 13, "out of range");
	checkError (replaceLine (VALID_INI, 13, "1stScalingC=99999999999999999999999"), 13,
				"must be a decimal");
}

BOOST_AUTO_TEST_CASE (scaling_d_is_zero)
{
	checkError (replaceLine (VALID_INI, 14, "1stScalingD=0"), 14, "cannot be zero");
	checkError (replaceLine (VALID_INI, 14, "1stScalingD=0x0"), 14, "cannot be zero");
}

BOOST_AUTO_TEST_SUITE_END ()
