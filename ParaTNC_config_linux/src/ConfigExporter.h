/*
 * ConfigExporter.h
 *
 * INI Configuration Exporter for libconfini
 * Exports ConfigVer0 configuration classes to INI format
 *
 * Created with libconfini integration
 */

#ifndef CONFIG_EXPORTER_H_
#define CONFIG_EXPORTER_H_

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include "../shared/config/ConfigVer0.h"

/**
 * ConfigExporter - Exports configuration to INI format compatible with libconfini
 * 
 * This class provides functionality to export ConfigurationManager objects
 * to INI format text files. Each configuration class gets its own [section],
 * and each setting includes a C++ type annotation in comments.
 */
class ConfigExporter {
public:
    /**
     * Constructor
     * @param configManager Reference to the ConfigurationManager instance
     */
    explicit ConfigExporter(std::shared_ptr<IConfigurationManager>);
    
    /**
     * Export configuration to INI file
     * @param filepath Path where to save the INI file
     * @return true if successful, false otherwise
     */
    bool exportToFile(const std::string& filepath);
    
    /**
     * Export configuration to string
     * @return INI formatted string
     */
    std::string exportToString();
    
private:
    std::shared_ptr<IConfigurationManager> configManager;
    std::ostringstream iniContent;
    
    // Export methods for each configuration section
    void exportBasicConfig();
    void exportModeConfig();
    void exportSourceConfig();
    void exportUmbConfig();
    void exportRtuConfig();
    void exportGsmConfig();
    
    // Helper methods
    void addSection(const std::string& sectionName);
    void addSetting(const std::string& key, uint8_t value, const std::string& typeComment);
    void addSetting(const std::string& key, uint16_t value, const std::string& typeComment);
    void addSetting(const std::string& key, float value, const std::string& typeComment);
    void addSetting(const std::string& key, bool value, const std::string& typeComment);
    void addSetting(const std::string& key, const std::string& value, const std::string& typeComment);
    
    std::string escapeValue(const std::string& value);
    std::string formatFloat(float value);
};

#endif /* CONFIG_EXPORTER_H_ */
