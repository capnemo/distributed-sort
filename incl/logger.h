#include <fstream>

#include <sys/time.h>
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>

#include <mutex>
#include <set>

#ifndef LOGGER_H
#define LOGGER_H

class logger {
    public:
    bool startLogger(const std::string& fileName);
    void addEntry(const std::string& line);
    void closeLogger() {close(logFd); open = false;}
    bool active() {return open;}

    private:
    std::ofstream outStr;
    int logFd;
    bool open = false;
    std::mutex logMtx;
    std::set<std::string> entries;
};
#endif /* LOGGER_H */
