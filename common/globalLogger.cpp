#include <iostream>


#include "logger.h"
#include "globalLogger.h"

logger gLog;

void globalLogger::initLogger(const std::string fileName)
{
    gLog.startLogger(fileName);
}

void globalLogger::logEntry(const std::string entry)
{
    if (gLog.active() == true)
        gLog.addEntry(entry);
    else
        std::cout << entry << std::endl;
}
