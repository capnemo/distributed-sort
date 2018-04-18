/***************************************************************************************
FILENAME: config.h

DESCRIPTION:

Class to read the config file. The config file is a text file containing key value pairs.
An object of this is meant to be used through globalConfig set of functions.

NOTES:

Not copy constructible or assignable. Not to be instantiated directly.
***************************************************************************************/


#ifndef MRCONFIG_H
#define MRCONFIG_H

#include <vector>
#include <string>
#include <map>

class mrConfig {
    
    public:
    mrConfig(){}
    mrConfig(const std::string& configFile):fileName(configFile) {}
    bool loadTable();
    bool setConfigFile(const std::string& fileName);
    void getValue(const std::string key, uint64_t& value) const;
    void getValue(const std::string key, std::string& value) const;
    mrConfig(const mrConfig&) = delete;
    mrConfig& operator = (const mrConfig&) = delete;
    ~mrConfig() = default;

    private:
    typedef std::map<std::string, std::string> table;
    typedef table::const_iterator tableIterator;
    std::string fileName;
    table configTable;
};
#endif /* MRCONFIG_H */
