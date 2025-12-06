/*
 * ConfigExporter.cpp
 *
 * Implementation of INI configuration exporter
 */

#include "ConfigExporter.h"
#include <iomanip>
#include <iostream>

ConfigExporter::ConfigExporter(std::shared_ptr<IConfigurationManager> configManager)
    : configManager(configManager) {}

bool ConfigExporter::exportToFile(const std::string& filepath) {
    try {
        iniContent.str("");
        iniContent.clear();
        
        // Generate INI content
        exportBasicConfig();
        exportModeConfig();
        exportSourceConfig();
        exportUmbConfig();
        exportRtuConfig();
        exportGsmConfig();
        
        // Write to file
        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file " << filepath << " for writing" << std::endl;
            return false;
        }

		std::cout << "I = ConfigExporter::exportToFile, " << filepath << std::endl;

        file << iniContent.str();
        file.close();
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception during export: " << e.what() << std::endl;
        return false;
    }
}

std::string ConfigExporter::exportToString() {
    iniContent.str("");
    iniContent.clear();
    
    exportBasicConfig();
    exportModeConfig();
    exportSourceConfig();
    exportUmbConfig();
    exportRtuConfig();
    exportGsmConfig();
    
    return iniContent.str();
}

void ConfigExporter::exportBasicConfig() {
    try {
        IBasicConfig& basic = configManager->getBasicConfig();
        addSection("BasicConfig");
        
        std::string callsign;
        basic.getCallsign(callsign);
        addSetting("callsign", callsign, "std::string");
        
        addSetting("ssid", basic.getSsid(), "uint8_t");
        addSetting("latitude", basic.getLatitude(), "float");
        addSetting("ns", basic.getNs(), "uint8_t");
        addSetting("longitude", basic.getLongitude(), "float");
        addSetting("we", basic.getWe(), "uint8_t");
        
        std::string comment;
        basic.getComment(comment);
        addSetting("comment", comment, "std::string");
        
        addSetting("symbol", basic.getSymbol(), "uint8_t");
        addSetting("pathType", basic.getPathType(), "uint8_t");
        addSetting("beaconBootup", basic.getBeaconBootup(), "bool");
        addSetting("wxTransmitPeriod", basic.getWxTransmitPeriod(), "uint8_t");
        addSetting("beaconTransmitPeriod", basic.getBeaconTransmitPeriod(), "uint8_t");
        addSetting("wxDoubleTransmit", basic.getWxDoubleTransmit(), "uint8_t");
        
        iniContent << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error exporting BasicConfig: " << e.what() << std::endl;
    }
}

void ConfigExporter::exportModeConfig() {
    try {
        IModeConfig& mode = configManager->getModeConfig();
        addSection("ModeConfig");
        
        addSetting("digi", mode.getDigi(), "uint8_t");
        addSetting("wx", mode.getWx(), "uint8_t");
        addSetting("wxUmb", mode.getWxUmb(), "uint8_t");
        addSetting("wxModbus", mode.getWxModbus(), "uint8_t");
        addSetting("wxDavis", mode.getWxDavis(), "uint8_t");
        addSetting("wxMs5611OrBme", mode.getWxMs5611OrBme(), "uint8_t");
        addSetting("wxAnemometerConst", mode.getWxAnemometerConst(), "uint8_t");
        addSetting("wxDustSensor", mode.getWxDustSensor(), "uint8_t");
        addSetting("wxPtSensor", mode.getWxPtSensor(), "uint8_t");
        addSetting("victron", mode.getVictron(), "uint8_t");
        addSetting("digiViscous", mode.getDigiViscous(), "uint8_t");
        addSetting("digiOnlySsid", mode.getDigiOnlySsid(), "uint8_t");
        addSetting("digiViscousDelay", mode.getDigiViscousDelay(), "uint8_t");
        addSetting("digiDelay100msec", mode.getDigiDelay100msec(), "uint8_t");
        addSetting("powersave", mode.getPowersave(), "uint8_t");
        addSetting("powersaveKeepGsm", mode.getPowersaveKeepGsm(), "uint8_t");
        addSetting("gsm", mode.getGsm(), "uint8_t");
        
        iniContent << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error exporting ModeConfig: " << e.what() << std::endl;
    }
}

void ConfigExporter::exportSourceConfig() {
    try {
        ISourceConfig& source = configManager->getSourceConfig();
        addSection("SourceConfig");
        
        addSetting("temperature", source.getTemperature(), "uint8_t");
        addSetting("pressure", source.getPressure(), "uint8_t");
        addSetting("humidity", source.getHumidity(), "uint8_t");
        addSetting("wind", source.getWind(), "uint8_t");
        
        iniContent << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error exporting SourceConfig: " << e.what() << std::endl;
    }
}

void ConfigExporter::exportUmbConfig() {
    try {
        IUmbConfig& umb = configManager->getUmbConfig();
        addSection("UmbConfig");
        
        addSetting("slaveClass", umb.getSlaveClass(), "uint16_t");
        addSetting("slaveId", umb.getSlaveId(), "uint16_t");
        addSetting("channelWindSpeed", umb.getChannelWindSpeed(), "uint16_t");
        addSetting("channelWindGust", umb.getChannelWindGust(), "uint16_t");
        addSetting("channelWindDirection", umb.getChannelWindDirection(), "uint16_t");
        addSetting("channelTemperature", umb.getChannelTemperature(), "uint16_t");
        addSetting("channelQnh", umb.getChannelQnh(), "uint16_t");
        
        iniContent << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error exporting UmbConfig: " << e.what() << std::endl;
    }
}

void ConfigExporter::exportRtuConfig() {
    try {
        IRtuConfig& rtu = configManager->getRtuConfig();
        addSection("RtuConfig");
        
        addSetting("slaveSpeed", rtu.getSlaveSpeed(), "uint16_t");
        addSetting("slaveParity", rtu.getSlaveParity(), "uint8_t");
        addSetting("slaveStopBits", rtu.getSlaveStopBits(), "uint8_t");
        addSetting("useFullWindData", rtu.getUseFullWindData(), "uint8_t");
        addSetting("temperatureSrc", rtu.getTemperatureSrc(), "uint8_t");
        addSetting("humiditySrc", rtu.getHumiditySrc(), "uint8_t");
        addSetting("pressureSrc", rtu.getPressureSrc(), "uint8_t");
        addSetting("windDir", rtu.getWindDir(), "uint8_t");
        addSetting("windSpeed", rtu.getWindSpeed(), "uint8_t");
        addSetting("windGusts", rtu.getWindGusts(), "uint8_t");
        
        iniContent << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error exporting RtuConfig: " << e.what() << std::endl;
    }
}

void ConfigExporter::exportGsmConfig() {
    try {
        IGsmConfig& gsm = configManager->getGsmConfig();
        addSection("GsmConfig");
        
        std::string pin, apn, username, password, apiUrl, stationName, server, passcode;
        gsm.getPin(pin);
        gsm.getApn(apn);
        gsm.getUsername(username);
        gsm.getPassword(password);
        gsm.getApiBaseUrl(apiUrl);
        gsm.getApiStationName(stationName);
        gsm.getAprsisServer(server);
        gsm.getAprsisPpasscode(passcode);
        
        addSetting("pin", pin, "std::string");
        addSetting("apn", apn, "std::string");
        addSetting("username", username, "std::string");
        addSetting("password", password, "std::string");
        addSetting("apiEnable", gsm.getApiEnable(), "uint8_t");
        addSetting("apiBaseUrl", apiUrl, "std::string");
        addSetting("apiStationName", stationName, "std::string");
        addSetting("aprsisEnable", gsm.getAprsisEnable(), "uint8_t");
        addSetting("aprsisServer", server, "std::string");
        addSetting("aprsisPort", gsm.getAprsisPort(), "uint16_t");
        addSetting("aprsisPpasscode", passcode, "std::string");
        
        iniContent << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error exporting GsmConfig: " << e.what() << std::endl;
    }
}

void ConfigExporter::addSection(const std::string& sectionName) {
    iniContent << "[" << sectionName << "]\n";
}

void ConfigExporter::addSetting(const std::string& key, uint8_t value, const std::string& typeComment) {
    iniContent << key << " = " << static_cast<int>(value) << " ; " << typeComment << "\n";
}

void ConfigExporter::addSetting(const std::string& key, uint16_t value, const std::string& typeComment) {
    iniContent << key << " = " << value << " ; " << typeComment << "\n";
}

void ConfigExporter::addSetting(const std::string& key, float value, const std::string& typeComment) {
    iniContent << key << " = " << formatFloat(value) << " ; " << typeComment << "\n";
}

void ConfigExporter::addSetting(const std::string& key, bool value, const std::string& typeComment) {
    iniContent << key << " = " << (value ? "true" : "false") << " ; " << typeComment << "\n";
}

void ConfigExporter::addSetting(const std::string& key, const std::string& value, const std::string& typeComment) {
    iniContent << key << " = " << escapeValue(value) << " ; " << typeComment << "\n";
}

std::string ConfigExporter::escapeValue(const std::string& value) {
    if (value.empty()) {
        return "\"\"";
    }
    
    // Check if value needs quoting
    bool needsQuotes = false;
    for (char c : value) {
        if (c == ' ' || c == '=' || c == ';' || c == '#') {
            needsQuotes = true;
            break;
        }
    }
    
    if (needsQuotes) {
        std::string escaped = "\"";
        for (char c : value) {
            if (c == '\\' || c == '"') {
                escaped += '\\';
            }
            escaped += c;
        }
        escaped += "\"";
        return escaped;
    }
    
    return value;
}

std::string ConfigExporter::formatFloat(float value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << value;
    std::string result = oss.str();
    
    // Remove trailing zeros
    size_t lastNonZero = result.find_last_not_of('0');
    if (lastNonZero != std::string::npos && result[lastNonZero] != '.') {
        result = result.substr(0, lastNonZero + 1);
    }
    
    return result;
}
