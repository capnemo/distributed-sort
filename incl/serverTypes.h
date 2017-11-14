#include "localTypes.h"

#ifndef SERVERTYPES_H
#define SERVERTYPES_H

struct task {
    char type;
    std::string id;
    strVec args;
};

struct result {
    std::string id;
    int16_t rc;
};

#endif /*SERVERTYPES_H*/
