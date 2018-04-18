/***************************************************************************************
FILENAME: globalLogger.h

DESCRIPTION:
    Set of helper functions to use the logger. The logger object is statically defined
    in globalLogger.cpp

NOTES:

***************************************************************************************/



#ifndef GLOBALLOGGER_H
#define GLOBALLOGGER_H

namespace globalLogger {
    void initLogger(const std::string fileName);
    //void logEntry(const std::string& entry);
    void logEntry(const std::string entry);
}

#endif /*GLOBALLOGGER_H */
