#include <iostream>
#include <fstream>

#include "config.h"

/***************************************************************
FUNCTION: mrConfig::loadTable()

Reads the config file and loads the key value store


****************************************************************/

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

/***************************************************************
FUNCTION: mrConfig::setConfigFile()
IN cfgFile: Name of the config file

Sets the config file.
****************************************************************/

bool mrConfig::setConfigFile(const std::string& cfgFile)
{
    fileName = cfgFile;
    return loadTable();
}

/***************************************************************
FUNCTION: mrConfig::getValue(const std::string 
IN key Key to be looked up.
OUT value of the key.

Returns the value of 'key' in value.

****************************************************************/

void mrConfig::getValue(const std::string key, uint64_t& value) const
{
    value = 0;
    std::string val;
    getValue(key, val);
    if (val != "")
        value = std::stoull(val);
}

/***************************************************************
FUNCTION: mrConfig::getValue(const std::string 
IN key Key to be looked up.
OUT value of the key.

Returns the value of 'key' in value.

****************************************************************/
void mrConfig::getValue(const std::string key, std::string& value) const
{
    value = "";
    tableIterator tIt = configTable.find(key);
    if (tIt != configTable.end())
        value = tIt->second;
}
