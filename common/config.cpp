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

void mrConfig::getValue(const std::string key, std::string& value) const
{
    value = "";
    tableIterator tit = configTable.find(key);
    if (tit != configTable.end())
        value = tit->second;
}

void mrConfig::getLogFileName(const mrConfig& conf, const std::string stub, 
                    std::string& fileName)
{
    std::string logFilePrefixValue;
    conf.getValue("logFilePrefix", logFilePrefixValue);

    std::string logFileValue;
    conf.getValue("logFileSuffix", logFileValue);

    fileName = logFilePrefixValue + stub + "-" + logFileValue;
}
