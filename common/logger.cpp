#include <sys/time.h>
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <iostream>

#include <mutex>
#include <set>
#include "logger.h"

bool logger::startLogger(const std::string& fileName)
{
    outStr.open(fileName);
    if (outStr.is_open() == false)
        return false;
    
    std::cout << "Log file name " << fileName << std::endl;
    open = true;
    return true;
}

void logger::addEntry(const std::string& line) 
{
    char currTime[128];
    time_t epochTime = time(0);
    std::string tStr(ctime_r(&epochTime, currTime));
    tStr.erase(tStr.size() - 1);

    std::string logLine = tStr + ":" + line;

    std::lock_guard<std::mutex> logLock(logMtx);
    if (outStr.is_open() == true)
        outStr << logLine << std::endl;
    else
        outStr << logLine << std::endl;
}

//Keep this. Someday locking over NFS will more "felicitious".
#if 0
void logger::startLogger() 
{ 
    logFd = open(fileName.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR |
                                                       S_IRGRP | S_IWGRP );
    if (logFd == -1)
        logFd = fileno(stdout);
}


void logger::startLogger() 
{ 
    logFd = open(fileName.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR |
                                                       S_IRGRP | S_IWGRP );
    if (logFd == -1)
        logFd = fileno(stdout);
}

void logger::addEntry(const std::string& line) 
{
    struct timeval tm;
    int rc = gettimeofday(&tm, 0);
    std::string prefix;
    if (rc == 0) {
        prefix = std::to_string(tm.tv_sec) +  "." + 
                 std::to_string(tm.tv_usec);
    } else {
        prefix = "------------------";
    }
    std::string logLine = prefix + ":" + line + '\n';
    
    std::lock_guard<std::mutex> logLock(logMtx);
    entries.insert(logLine);
    if (entries.size() > bufferSize) {
        int rc = lockf(logFd, F_LOCK, 0);
        if (rc == 0) {
            for (auto mem:entries)
                write(logFd, mem.c_str(), mem.size());
            entries.clear();
            lockf(logFd, F_ULOCK, 0);
        }
    }
}

#endif
