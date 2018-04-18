#include <iostream>


#include "logger.h"
#include "globalLogger.h"

logger gLog;


/***************************************************************
FUNCTION: globalLogger::initLogger
IN: fileName log file name.

Start the logger
****************************************************************/

void globalLogger::initLogger(const std::string fileName)
{
    gLog.startLogger(fileName);
}

/***************************************************************
FUNCTION: globalLogger::logEntry
IN: entry

Write the entry to the logfile.
****************************************************************/
void globalLogger::logEntry(const std::string entry)
{
    if (gLog.active() == true)
        gLog.addEntry(entry);
    else
        std::cout << entry << std::endl;
}
