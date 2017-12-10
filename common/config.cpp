#include <iostream>
#include <fstream>

#include "config.h"

bool mrConfig::loadTable()
{
    std::ifstream cfgFp(fileName);
    std::string key, value;
    
    if (cfgFp.is_open() == false)
        return false;

    while (cfgFp) {
        cfgFp >> key >> value;
        if (key[0] != '#')
            configTable[key] = value;
    }

    return true;
}

bool mrConfig::setConfigFile(const std::string& cfgFile)
{
    fileName = cfgFile;
    return loadTable();
}

void mrConfig::getValue(const std::string key, uint64_t& value) const
{
    value = 0;
    std::string val;
    getValue(key, val);
    if (val != "")
        value = std::stoull(val);
}

void mrConfig::getValue(const std::string key, std::string& value) const
{
    value = "";
    tableIterator tIt = configTable.find(key);
    if (tIt != configTable.end())
        value = tIt->second;
}
