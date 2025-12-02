/*
 * ConfigImporter.cpp
 *
 * Implementation of INI configuration importer
 */

#include "ConfigImporter.h"

ConfigImporter::ConfigImporter(ConfigurationManager& configManager)
    : configManager(configManager), currentSection("") {}

bool ConfigImporter::importFromFile(const std::string& filepath) {
    try {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file " << filepath << " for reading" << std::endl;
            return false;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        
        return importFromString(buffer.str());
    } catch (const std::exception& e) {
        std::cerr << "Exception during import: " << e.what() << std::endl;
        return false;
    }
}

bool ConfigImporter::importFromString(const std::string& content) {
    try {
        currentSection = "";
        std::istringstream iss(content);
        std::string line;
        
        while (std::getline(iss, line)) {
            // Remove comments
            line = stripComment(line);
            // Trim whitespace
            line = trim(line);
            
            // Skip empty lines
            if (line.empty()) {
                continue;
            }
            
            // Check if it's a section header
            if (line[0] == '[') {
                parseSection(line);
            } else {
                // Parse as setting
                parseSetting(line);
            }
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception during import: " << e.what() << std::endl;
        return false;
    }
}

std::string ConfigImporter::toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string ConfigImporter::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, (end - start + 1));
}

std::string ConfigImporter::stripComment(const std::string& line) {
    size_t semiPos = line.find(';');
    size_t hashPos = line.find('#');
    
    size_t commentPos = std::string::npos;
    if (semiPos != std::string::npos && hashPos != std::string::npos) {
        commentPos = std::min(semiPos, hashPos);
    } else if (semiPos != std::string::npos) {
        commentPos = semiPos;
    } else if (hashPos != std::string::npos) {
        commentPos = hashPos;
    }
    
    if (commentPos != std::string::npos) {
        return line.substr(0, commentPos);
    }
    
    return line;
}

bool ConfigImporter::parseSection(const std::string& line) {
    if (line[0] != '[' || line[line.length() - 1] != ']') {
        return false;
    }
    
    currentSection = line.substr(1, line.length() - 2);
    currentSection = toLowerCase(trim(currentSection));
    return true;
}

bool ConfigImporter::parseSetting(const std::string& line) {
    size_t equalPos = line.find('=');
    if (equalPos == std::string::npos) {
        return false;
    }
    
    std::string key = line.substr(0, equalPos);
    std::string value = line.substr(equalPos + 1);
    
    key = toLowerCase(trim(key));
    value = trim(value);
    
    if (currentSection == "basicconfig") {
        parseBasicConfigSetting(key, value);
    } else if (currentSection == "modeconfig") {
        parseModeConfigSetting(key, value);
    } else if (currentSection == "sourceconfig") {
        parseSourceConfigSetting(key, value);
    } else if (currentSection == "umbconfig") {
        parseUmbConfigSetting(key, value);
    } else if (currentSection == "rtuconfig") {
        parseRtuConfigSetting(key, value);
    } else if (currentSection == "gsmconfig") {
        parseGsmConfigSetting(key, value);
    }
    
    return true;
}

void ConfigImporter::parseBasicConfigSetting(const std::string& key, const std::string& value) {
    IBasicConfig& basic = configManager.getBasicConfig();
    
    if (key == "callsign") {
        basic.setCallsign(parseString(value));
    } else if (key == "ssid") {
        basic.setSsid(parseUint8(value));
    } else if (key == "latitude") {
        basic.setLatitude(parseFloat(value));
    } else if (key == "ns") {
        basic.setNs(parseUint8(value));
    } else if (key == "longitude") {
        basic.setLongitude(parseFloat(value));
    } else if (key == "we") {
        basic.setWe(parseUint8(value));
    } else if (key == "comment") {
        basic.setComment(parseString(value));
    } else if (key == "symbol") {
        basic.setSymbol(parseUint8(value));
    } else if (key == "pathtype") {
        basic.setPathType(parseUint8(value));
    } else if (key == "beaconbootup") {
        basic.setBeaconBootup(parseBoolean(value));
    } else if (key == "wxtransmitperiod") {
        basic.setWxTransmitPeriod(parseUint8(value));
    } else if (key == "beacontransmitperiod") {
        basic.setBeaconTransmitPeriod(parseUint8(value));
    } else if (key == "wxdoubletransmit") {
        basic.setWxDoubleTransmit(parseUint8(value));
    }
}

void ConfigImporter::parseModeConfigSetting(const std::string& key, const std::string& value) {
    IModeConfig& mode = configManager.getModeConfig();
    
    if (key == "digi") {
        mode.setDigi(parseUint8(value));
    } else if (key == "wx") {
        mode.setWx(parseUint8(value));
    } else if (key == "wxumb") {
        mode.setWxUmb(parseUint8(value));
    } else if (key == "wxmodbus") {
        mode.setWxModbus(parseUint8(value));
    } else if (key == "wxdavis") {
        mode.setWxDavis(parseUint8(value));
    } else if (key == "wxms5611orbme") {
        mode.setWxMs5611OrBme(parseUint8(value));
    } else if (key == "wxanemometerconst") {
        mode.setWxAnemometerConst(parseUint8(value));
    } else if (key == "wxdustsensor") {
        mode.setWxDustSensor(parseUint8(value));
    } else if (key == "wxptsensor") {
        mode.setWxPtSensor(parseUint8(value));
    } else if (key == "victron") {
        mode.setVictron(parseUint8(value));
    } else if (key == "digiviscous") {
        mode.setDigiViscous(parseUint8(value));
    } else if (key == "digionlyssid") {
        mode.setDigiOnlySsid(parseUint8(value));
    } else if (key == "digiviscousdelay") {
        mode.setDigiViscousDelay(parseUint8(value));
    } else if (key == "digidelay100msec") {
        mode.setDigiDelay100msec(parseUint8(value));
    } else if (key == "powersave") {
        mode.setPowersave(parseUint8(value));
    } else if (key == "powersavekeepgsm") {
        mode.setPowersaveKeepGsm(parseUint8(value));
    } else if (key == "gsm") {
        mode.setGsm(parseUint8(value));
    }
}

void ConfigImporter::parseSourceConfigSetting(const std::string& key, const std::string& value) {
    ISourceConfig& source = configManager.getSourceConfig();
    
    if (key == "temperature") {
        source.setTemperature(parseUint8(value));
    } else if (key == "pressure") {
        source.setPressure(parseUint8(value));
    } else if (key == "humidity") {
        source.setHumidity(parseUint8(value));
    } else if (key == "wind") {
        source.setWind(parseUint8(value));
    }
}

void ConfigImporter::parseUmbConfigSetting(const std::string& key, const std::string& value) {
    IUmbConfig& umb = configManager.getUmbConfig();
    
    if (key == "slaveclass") {
        umb.setSlaveClass(parseUint16(value));
    } else if (key == "slaveid") {
        umb.setSlaveId(parseUint16(value));
    } else if (key == "channelwindspeed") {
        umb.setChannelWindSpeed(parseUint16(value));
    } else if (key == "channelwindgust") {
        umb.setChannelWindGust(parseUint16(value));
    } else if (key == "channelwinddirection") {
        umb.setChannelWindDirection(parseUint16(value));
    } else if (key == "channeltemperature") {
        umb.setChannelTemperature(parseUint16(value));
    } else if (key == "channelqnh") {
        umb.setChannelQnh(parseUint16(value));
    }
}

void ConfigImporter::parseRtuConfigSetting(const std::string& key, const std::string& value) {
    IRtuConfig& rtu = configManager.getRtuConfig();
    
    if (key == "slavespeed") {
        rtu.setSlaveSpeed(parseUint16(value));
    } else if (key == "slaveparity") {
        rtu.setSlaveParity(parseUint8(value));
    } else if (key == "slaveStopbits") {
        rtu.setSlaveStopBits(parseUint8(value));
    } else if (key == "usefullwinddata") {
        rtu.setUseFullWindData(parseUint8(value));
    } else if (key == "temperaturesrc") {
        rtu.setTemperatureSrc(parseUint8(value));
    } else if (key == "humiditysrc") {
        rtu.setHumiditySrc(parseUint8(value));
    } else if (key == "pressuresrc") {
        rtu.setPressureSrc(parseUint8(value));
    } else if (key == "winddir") {
        rtu.setWindDir(parseUint8(value));
    } else if (key == "windspeed") {
        rtu.setWindSpeed(parseUint8(value));
    } else if (key == "windgusts") {
        rtu.setWindGusts(parseUint8(value));
    }
}

void ConfigImporter::parseGsmConfigSetting(const std::string& key, const std::string& value) {
    IGsmConfig& gsm = configManager.getGsmConfig();
    
    if (key == "pin") {
        gsm.setPin(parseString(value));
    } else if (key == "apn") {
        gsm.setApn(parseString(value));
    } else if (key == "username") {
        gsm.setUsername(parseString(value));
    } else if (key == "password") {
        gsm.setPassword(parseString(value));
    } else if (key == "apienable") {
        gsm.setApiEnable(parseUint8(value));
    } else if (key == "apibaseurl") {
        gsm.setApiBaseUrl(parseString(value));
    } else if (key == "apistationname") {
        gsm.setApiStationName(parseString(value));
    } else if (key == "aprsisenabled") {
        gsm.setAprsisEnable(parseUint8(value));
    } else if (key == "aprsisserver") {
        gsm.setAprsisServer(parseString(value));
    } else if (key == "aprsisport") {
        gsm.setAprsisPort(parseUint16(value));
    } else if (key == "aprsipasscode") {
        gsm.setAprsisPpasscode(parseString(value));
    }
}

uint8_t ConfigImporter::parseUint8(const std::string& value) {
    try {
        int val = std::stoi(value);
        return static_cast<uint8_t>(val & 0xFF);
    } catch (...) {
        return 0;
    }
}

uint16_t ConfigImporter::parseUint16(const std::string& value) {
    try {
        int val = std::stoi(value);
        return static_cast<uint16_t>(val & 0xFFFF);
    } catch (...) {
        return 0;
    }
}

float ConfigImporter::parseFloat(const std::string& value) {
    try {
        return std::stof(value);
    } catch (...) {
        return 0.0f;
    }
}

bool ConfigImporter::parseBoolean(const std::string& value) {
    std::string lowercaseValue = toLowerCase(value);
    
    if (lowercaseValue == "true" || lowercaseValue == "1" || lowercaseValue == "yes" || lowercaseValue == "on") {
        return true;
    }
    
    return false;
}

std::string ConfigImporter::parseString(const std::string& value) {
    // Remove surrounding quotes if present
    std::string result = value;
    if (result.length() >= 2 && result[0] == '"' && result[result.length() - 1] == '"') {
        result = result.substr(1, result.length() - 2);
    }
    
    // Handle escaped characters
    std::string unescaped;
    for (size_t i = 0; i < result.length(); ++i) {
        if (result[i] == '\\' && i + 1 < result.length()) {
            char nextChar = result[i + 1];
            if (nextChar == '\\' || nextChar == '"') {
                unescaped += nextChar;
                ++i;
            } else {
                unescaped += result[i];
            }
        } else {
            unescaped += result[i];
        }
    }
    
    return unescaped;
}
