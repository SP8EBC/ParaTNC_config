/*
 * ConfigImporter.cpp
 *
 * Implementation of INI configuration importer
 */

#include "ConfigImporter.h"

#define CONFIG_IMPORTE_SET_FLAG(flag, bitpos)			\
		flag |= (1 << bitpos)							\

ConfigImporter::ConfigImporter(std::shared_ptr<IConfigurationManager> configManager)
    : configManager(configManager), currentSection(""),
	  basicFlags(0), modeFlags(0), sourceFlags(0), umbFlags(0),
	  rtuFlags(0), gsmFlags(0){}

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

bool ConfigImporter::allSet() {
	bool result = false;

	const bool basicResult = checkFlags(basicFlags, 12);
	const bool modeResult = checkFlags(modeFlags, 16);
	const bool sourceResult = checkFlags(sourceFlags, 3);
	const bool umbResult = checkFlags(umbFlags, 6);
	const bool rtuResult = checkFlags(rtuFlags, 9);
	const bool gsmResult = checkFlags(gsmFlags, 10);

	if (basicResult || modeResult || sourceResult || umbResult || rtuResult || gsmResult) {
		result = true;
	}

	return result;
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
    } else {
    	// rtu slave sections

    	// checks if section name contains digits - currently only 6 slaves
    	// are supported by ParaMETEO
    	std::size_t found = currentSection.find_first_of("012345");

    	// digits have been found
    	if (found != std::string::npos) {
    		const std::string prefix = currentSection.substr(0, found);
    		const int slaveId = atoi(currentSection.substr(found, 1).c_str());
    		if (prefix == "rtuslave") {
    			parseRtuSlaveConfigSetting(slaveId, key, value);
    		}
    	}
    }
    
    return true;
}

void ConfigImporter::parseBasicConfigSetting(const std::string& key, const std::string& value) {
    IBasicConfig& basic = configManager->getBasicConfig();
    
    if (key == "callsign") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 0);
        basic.setCallsign(parseString(value));
    } else if (key == "ssid") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 1);
        basic.setSsid(parseUint8(value));
    } else if (key == "latitude") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 2);
        basic.setLatitude(parseFloat(value));
    } else if (key == "ns") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 3);
    	const uint8_t ns = parseString(value)[0];
    	if (ns == 'n' || ns == 'N' || ns == 's' || ns == 'S')
    	{
        	basic.setNs(ns);
    	}
    	else
    	{
			std::cout << "E = ConfigImporter::parseBasicConfigSetting, value: " << value << ", ns: " << (int)ns << std::endl;
    		throw std::out_of_range("Wrong value for 'ns' setting in [Basic] section");
    	}
    } else if (key == "longitude") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 4);
        basic.setLongitude(parseFloat(value));
    } else if (key == "we") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 5);
    	const uint8_t we = parseString(value)[0];
    	if (we == 'w' || we == 'W' || we == 'e' || we == 'E')
    	{
    		basic.setWe(we);
    	}
    	else
    	{
			std::cout << "E = ConfigImporter::parseBasicConfigSetting, value: " << value << ", we: " << (int)we << std::endl;
    		throw std::out_of_range("Wrong value for 'we' setting in [Basic] section");
    	}
    } else if (key == "comment") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 6);
        basic.setComment(parseString(value));
    } else if (key == "symbol") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 7);
        basic.setSymbol(parseUint8(value));
    } else if (key == "pathtype") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 8);
        basic.setPathType(parseUint8(value));
    } else if (key == "beaconbootup") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 9);
        basic.setBeaconBootup(parseBoolean(value));
    } else if (key == "wxtransmitperiod") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 10);
        basic.setWxTransmitPeriod(parseUint8(value));
    } else if (key == "beacontransmitperiod") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 11);
        basic.setBeaconTransmitPeriod(parseUint8(value));
    } else if (key == "wxdoubletransmit") {
    	CONFIG_IMPORTE_SET_FLAG(basicFlags, 12);
        basic.setWxDoubleTransmit(parseUint8(value));
    }
}

void ConfigImporter::parseModeConfigSetting(const std::string& key, const std::string& value) {
    IModeConfig& mode = configManager->getModeConfig();
    
    if (key == "digi") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 0);
        mode.setDigi(parseUint8(value));
    } else if (key == "wx") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 1);
        mode.setWx(parseUint8(value));
    } else if (key == "wxumb") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 2);
        mode.setWxUmb(parseUint8(value));
    } else if (key == "wxmodbus") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 3);
        mode.setWxModbus(parseUint8(value));
    } else if (key == "wxdavis") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 4);
        mode.setWxDavis(parseUint8(value));
    } else if (key == "wxms5611orbme") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 5);
        mode.setWxMs5611OrBme(parseUint8(value));
    } else if (key == "wxanemometerconst") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 6);
        mode.setWxAnemometerConst(parseUint8(value));
    } else if (key == "wxdustsensor") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 7);
        mode.setWxDustSensor(parseUint8(value));
    } else if (key == "wxptsensor") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 8);
        mode.setWxPtSensor(parseUint8(value));
    } else if (key == "victron") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 9);
        mode.setVictron(parseUint8(value));
    } else if (key == "digiviscous") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 10);
        mode.setDigiViscous(parseUint8(value));
    } else if (key == "digionlyssid") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 11);
        mode.setDigiOnlySsid(parseUint8(value));
    } else if (key == "digiviscousdelay") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 12);
        mode.setDigiViscousDelay(parseUint8(value));
    } else if (key == "digidelay100msec") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 13);
        mode.setDigiDelay100msec(parseUint8(value));
    } else if (key == "powersave") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 14);
        mode.setPowersave(parseUint8(value));
    } else if (key == "powersavekeepgsm") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 15);
        mode.setPowersaveKeepGsm(parseUint8(value));
    } else if (key == "gsm") {
    	CONFIG_IMPORTE_SET_FLAG(modeFlags, 16);
        mode.setGsm(parseUint8(value));
    }
}

void ConfigImporter::parseSourceConfigSetting(const std::string& key, const std::string& value) {
    ISourceConfig& source = configManager->getSourceConfig();
    
    if (key == "temperature") {
    	CONFIG_IMPORTE_SET_FLAG(sourceFlags, 0);
        source.setTemperature(parseUint8(value));
    } else if (key == "pressure") {
    	CONFIG_IMPORTE_SET_FLAG(sourceFlags, 1);
        source.setPressure(parseUint8(value));
    } else if (key == "humidity") {
    	CONFIG_IMPORTE_SET_FLAG(sourceFlags, 2);
        source.setHumidity(parseUint8(value));
    } else if (key == "wind") {
    	CONFIG_IMPORTE_SET_FLAG(sourceFlags, 3);
        source.setWind(parseUint8(value));
    }
}

void ConfigImporter::parseUmbConfigSetting(const std::string& key, const std::string& value) {
    IUmbConfig& umb = configManager->getUmbConfig();
    
    if (key == "slaveclass") {
    	CONFIG_IMPORTE_SET_FLAG(umbFlags, 0);
        umb.setSlaveClass(parseUint16(value));
    } else if (key == "slaveid") {
    	CONFIG_IMPORTE_SET_FLAG(umbFlags, 1);
        umb.setSlaveId(parseUint16(value));
    } else if (key == "channelwindspeed") {
    	CONFIG_IMPORTE_SET_FLAG(umbFlags, 2);
        umb.setChannelWindSpeed(parseUint16(value));
    } else if (key == "channelwindgust") {
    	CONFIG_IMPORTE_SET_FLAG(umbFlags, 3);
        umb.setChannelWindGust(parseUint16(value));
    } else if (key == "channelwinddirection") {
    	CONFIG_IMPORTE_SET_FLAG(umbFlags, 4);
        umb.setChannelWindDirection(parseUint16(value));
    } else if (key == "channeltemperature") {
    	CONFIG_IMPORTE_SET_FLAG(umbFlags, 5);
        umb.setChannelTemperature(parseUint16(value));
    } else if (key == "channelqnh") {
    	CONFIG_IMPORTE_SET_FLAG(umbFlags, 6);
        umb.setChannelQnh(parseUint16(value));
    }
}

void ConfigImporter::parseRtuConfigSetting(const std::string& key, const std::string& value) {
    IRtuConfig& rtu = configManager->getRtuConfig();
    
    if (key == "slavespeed") {
    	CONFIG_IMPORTE_SET_FLAG(rtuFlags, 0);
        rtu.setSlaveSpeed(parseUint16(value));
    } else if (key == "slaveparity") {
    	CONFIG_IMPORTE_SET_FLAG(rtuFlags, 1);
        rtu.setSlaveParity(parseUint8(value));
    } else if (key == "slaveStopbits") {
    	CONFIG_IMPORTE_SET_FLAG(rtuFlags, 2);
        rtu.setSlaveStopBits(parseUint8(value));
    } else if (key == "usefullwinddata") {
    	CONFIG_IMPORTE_SET_FLAG(rtuFlags, 3);
        rtu.setUseFullWindData(parseUint8(value));
    } else if (key == "temperaturesrc") {
    	CONFIG_IMPORTE_SET_FLAG(rtuFlags, 4);
        rtu.setTemperatureSrc(parseUint8(value));
    } else if (key == "humiditysrc") {
    	CONFIG_IMPORTE_SET_FLAG(rtuFlags, 5);
        rtu.setHumiditySrc(parseUint8(value));
    } else if (key == "pressuresrc") {
    	CONFIG_IMPORTE_SET_FLAG(rtuFlags, 6);
        rtu.setPressureSrc(parseUint8(value));
    } else if (key == "winddir") {
    	CONFIG_IMPORTE_SET_FLAG(rtuFlags, 7);
        rtu.setWindDir(parseUint8(value));
    } else if (key == "windspeed") {
    	CONFIG_IMPORTE_SET_FLAG(rtuFlags, 8);
        rtu.setWindSpeed(parseUint8(value));
    } else if (key == "windgusts") {
    	CONFIG_IMPORTE_SET_FLAG(rtuFlags, 9);
        rtu.setWindGusts(parseUint8(value));
    }
}

void ConfigImporter::parseRtuSlaveConfigSetting(uint8_t id, const std::string& key, const std::string& value)
{
	IRtuConfig &rtu = configManager->getRtuConfig ();

	auto slave = rtu.getSlave (id);

	if (key == "busaddress") {
		slave.busAddress = (parseUint8 (value));
	}
	else if (key == "function") {
		slave.function = (parseUint8 (value));

		if ((slave.function != 0x03) && (slave.function != 0x04)) {
			std::cout << "E = ConfigImporter::parseRtuSlaveConfigSetting, id: " << (int)id
					  << ", potentially unsupported Modbus function!! " << std::endl;
		}
	}
	else if (key == "readlen") {
		slave.readLen = (parseUint16 (value));
	}
	else if (key == "registeraddress") {
		slave.registerAddress = (parseUint16 (value));
	}
	else if (key == "scalinga") {
		slave.scalingA = (parseUint8 (value));
	}
	else if (key == "scalingb") {
		slave.scalingB = (parseUint8 (value));
	}
	else if (key == "scalingc") {
		slave.scalingC = (parseUint8 (value));
	}
	else if (key == "scalingd") {
		slave.scalingD = (parseUint8 (value));
		if (slave.scalingD == 0) {
			throw std::out_of_range (
				"scalingD in RTU slave config cannot equals zero, as this will cause div/0 error");
		}
	}
	else if (key == "signedvalue") {
		slave.signedValue = (parseUint8 (value));
	}
	else {
		throw std::runtime_error("unknown option in RTU slave config.");
	}

	rtu.setSlave(id, slave);

/**
 * 		addSetting("busAddress", slave.busAddress, " ");
		addSetting("function", slave.function, " ");
		addSetting("readLen", slave.readLen, " ");
		addSetting("registerAddress", slave.registerAddress, " ");
		addSetting("scalingA", slave.scalingA, " ");
		addSetting("scalingB", slave.scalingB, " ");
		addSetting("scalingC", slave.scalingC, " ");
		addSetting("scalingD", slave.scalingD, " ");
		addSetting("signedValue", slave.signedValue, " ");
 *
 */
}

void ConfigImporter::parseGsmConfigSetting(const std::string& key, const std::string& value) {
    IGsmConfig& gsm = configManager->getGsmConfig();
    
    if (key == "pin") {
    	CONFIG_IMPORTE_SET_FLAG(gsmFlags, 0);
        gsm.setPin(parseString(value));
    } else if (key == "apn") {
    	CONFIG_IMPORTE_SET_FLAG(gsmFlags, 1);
        gsm.setApn(parseString(value));
    } else if (key == "username") {
    	CONFIG_IMPORTE_SET_FLAG(gsmFlags, 2);
        gsm.setUsername(parseString(value));
    } else if (key == "password") {
    	CONFIG_IMPORTE_SET_FLAG(gsmFlags, 3);
        gsm.setPassword(parseString(value));
    } else if (key == "apienable") {
    	CONFIG_IMPORTE_SET_FLAG(gsmFlags, 4);
        gsm.setApiEnable(parseUint8(value));
    } else if (key == "apibaseurl") {
    	CONFIG_IMPORTE_SET_FLAG(gsmFlags, 5);
        gsm.setApiBaseUrl(parseString(value));
    } else if (key == "apistationname") {
    	CONFIG_IMPORTE_SET_FLAG(gsmFlags, 6);
        gsm.setApiStationName(parseString(value));
    } else if (key == "aprsisenabled") {
    	CONFIG_IMPORTE_SET_FLAG(gsmFlags, 7);
        gsm.setAprsisEnable(parseUint8(value));
    } else if (key == "aprsisserver") {
    	CONFIG_IMPORTE_SET_FLAG(gsmFlags, 8);
        gsm.setAprsisServer(parseString(value));
    } else if (key == "aprsisport") {
    	CONFIG_IMPORTE_SET_FLAG(gsmFlags, 9);
        gsm.setAprsisPort(parseUint16(value));
    } else if (key == "aprsipasscode") {
    	CONFIG_IMPORTE_SET_FLAG(gsmFlags, 10);
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
    else if (lowercaseValue == "false" || lowercaseValue == "0" || lowercaseValue == "no" || lowercaseValue == "off") {
        return true;
    }
    else {
    	throw std::runtime_error("Error parsing boolean value from line: " + value);
    }
    
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
