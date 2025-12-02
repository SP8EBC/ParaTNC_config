/*
 * ConfigImporter.h
 *
 * INI Configuration Importer for libconfini
 * Reads INI files and populates ConfigVer0 configuration classes
 *
 * Created with libconfini integration
 */

#ifndef CONFIG_IMPORTER_H_
#define CONFIG_IMPORTER_H_

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include "../shared/config/ConfigVer0.h"

/**
 * ConfigImporter - Imports configuration from INI format
 * 
 * This class provides functionality to read INI format text files and
 * populate ConfigurationManager objects. Parsing is case-insensitive
 * for sections and keys, but preserves case for string values.
 */
class ConfigImporter {
public:
    /**
     * Constructor
     * @param configManager Reference to the ConfigurationManager instance to populate
     */
    explicit ConfigImporter(ConfigurationManager& configManager);
    
    /**
     * Import configuration from INI file
     * @param filepath Path to the INI file to read
     * @return true if successful, false otherwise
     */
    bool importFromFile(const std::string& filepath);
    
    /**
     * Import configuration from string
     * @param content INI formatted string content
     * @return true if successful, false otherwise
     */
    bool importFromString(const std::string& content);
    
private:
    ConfigurationManager& configManager;
    std::string currentSection;
    
    // Helper methods
    std::string toLowerCase(const std::string& str);
    std::string trim(const std::string& str);
    std::string stripComment(const std::string& line);
    bool parseSection(const std::string& line);
    bool parseSetting(const std::string& line);
    
    // Section parsers
    void parseBasicConfigSetting(const std::string& key, const std::string& value);
    void parseModeConfigSetting(const std::string& key, const std::string& value);
    void parseSourceConfigSetting(const std::string& key, const std::string& value);
    void parseUmbConfigSetting(const std::string& key, const std::string& value);
    void parseRtuConfigSetting(const std::string& key, const std::string& value);
    void parseGsmConfigSetting(const std::string& key, const std::string& value);
    
    // Type conversion helpers
    uint8_t parseUint8(const std::string& value);
    uint16_t parseUint16(const std::string& value);
    float parseFloat(const std::string& value);
    bool parseBoolean(const std::string& value);
    std::string parseString(const std::string& value);
};

#endif /* CONFIG_IMPORTER_H_ */
