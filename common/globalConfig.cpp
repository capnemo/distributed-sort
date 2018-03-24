#include "config.h"
#include "globalConfig.h"

static mrConfig cfg;

bool globalConfig::initConfig(std::string fileName)
{
    return cfg.setConfigFile(fileName);
}

void globalConfig::getValueByKey(std::string key, std::string& value)
{
    cfg.getValue(key, value);
}

void globalConfig::getValueByKey(std::string key, uint64_t& value)
{
    cfg.getValue(key, value);
}


void globalConfig::getLogFileName(const std::string stub, std::string& fileName)
{
    std::string logFilePrefixValue;
    cfg.getValue("logFilePrefix", logFilePrefixValue);

    
    fileName = logFilePrefixValue + "/" + stub + ".log";
    /*
    std::string logFileValue;
    cfg.getValue("logFileSuffix", logFileValue);

    fileName = logFilePrefixValue + stub + "-" + logFileValue;
    */
}
