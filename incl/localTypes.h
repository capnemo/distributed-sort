/***************************************************************************************
FILENAME: localTypes.h

DESCRIPTION:
    Types for the agent side.

NOTES:

***************************************************************************************/


#ifndef LOCALTYPES_H
#define LOCALTYPES_H

#include <cstdint>
#include <vector>
#include <string>
#include <ctime>

typedef std::vector<std::string> strVec;
typedef std::pair<uint64_t, uint64_t> offPair;
typedef std::vector<offPair> offPairVec;

struct subTask {
    std::string id;
    int (*func)(const strVec& args);
    strVec args;
    time_t startTime;
    time_t endTime;
    int result;
};
typedef std::vector<struct subTask> taskVec;

#endif /*LOCALTYPES_H*/
