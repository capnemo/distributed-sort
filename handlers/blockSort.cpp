#include <fstream>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <ctime>
#include "blockSort.h"
#include "globalLogger.h"

struct charPtrCmp {
    bool operator() (const char *s1, const char* s2) {
        return (strcmp(s1,s2) < 0) ? true: false;
    }
};

void blockSort::operator() () 
{
    std::string idStr = "Sort Block " + std::to_string(id);
    globalLogger::logEntry("Starting " + idStr);
    time_t startTime = time(nullptr);

    bool rc = sort();
    if (rc == true)  {
        std::string timeStr = std::to_string(time(nullptr) - startTime);
        globalLogger::logEntry(idStr + " completed in:" + timeStr);
    } else 
        globalLogger::logEntry(idStr + " failed");
}

int blockSort::sort()
{
    std::ifstream inFileStr(args[0]);

    if (inFileStr.is_open() == false) {
        globalLogger::logEntry(args[0] + " in file not open");
	return false;
    }

    uint64_t beginRange = std::stoll(args[1]);
    uint64_t endRange = std::stoll(args[2]);
    uint64_t buffSz = endRange - beginRange;

    buffer = new char[buffSz];
    inFileStr.seekg(beginRange);
    inFileStr.read(buffer, buffSz);
    uint32_t pos = 0;
    recVec.push_back(buffer + pos);

    for(pos = 0; pos < buffSz - 1; pos++) {
        if (buffer[pos] == '\n') {
            buffer[pos] = '\0';
            recVec.push_back(buffer + pos + 1);
        }
    }
    buffer[buffSz - 1] = '\0';

    charPtrCmp comp;
    std::sort(recVec.begin(), recVec.end(), comp);
    return true;
}

void blockSort::getRecords(charPtrVec& rv)
{
    rv = recVec;
}

blockSort::~blockSort()
{
    delete [] buffer;
}
