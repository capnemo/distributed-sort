
#ifndef GLOBALLOGGER_H
#define GLOBALLOGGER_H

namespace globalLogger {
    void initLogger(const std::string fileName);
    //void logEntry(const std::string& entry);
    void logEntry(const std::string entry);
}

#endif /*GLOBALLOGGER_H */
