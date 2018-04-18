#include "config.h"
#include "globalConfig.h"

static mrConfig cfg;


/***************************************************************
FUNCTION: globalConfig::initConfig
IN: fileName Input file name

Sets the config file name.
****************************************************************/

bool globalConfig::initConfig(std::string fileName)
{
    return cfg.setConfigFile(fileName);
}

/***************************************************************
FUNCTION: globalConfig::getValueByKey
IN: key 
OUT: value

Returns the value of the key.

****************************************************************/

void globalConfig::getValueByKey(std::string key, std::string& value)
{
    cfg.getValue(key, value);
}

/***************************************************************
FUNCTION: globalConfig::getValueByKey
IN: key 
OUT: value

Returns the value of the key.

****************************************************************/
void globalConfig::getValueByKey(std::string key, uint64_t& value)
{
    cfg.getValue(key, value);
}


/***************************************************************
FUNCTION: globalConfig::getLogFileName
IN: stub  Filename without the extension.
OUT: fileName Absolute filename with the extension

****************************************************************/

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
