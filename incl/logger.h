/***************************************************************************************
FILENAME: logger.h

DESCRIPTION:
    Class for logging. Works on the agent and the initiator side.
    The actual write to the log file is protected by a mutex because there are multiple 
    threads that use a single instance of this class.
    Works on both the agent and initiator sides for different log files. 


NOTES:
    Cannot be copy constructed and cannot be assigned.

***************************************************************************************/


#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <sys/time.h>
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>

#include <mutex>
#include <set>

class logger {
    public:
    logger() = default;
    bool startLogger(const std::string& fileName);
    void addEntry(const std::string& line);
    void closeLogger() {close(logFd); open = false;}
    bool active() {return open;}

    logger(const logger&) = delete;
    logger& operator = (const logger&) = delete;
    ~logger() = default;

    private:
    std::ofstream outStr;
    int logFd;
    bool open = false;
    std::mutex logMtx;
    std::set<std::string> entries;
};
#endif /* LOGGER_H */
