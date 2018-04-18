/***************************************************************************************
FILENAME: serverTypes.h

DESCRIPTION:
    Datatypes for the initiator side. 

NOTES:

***************************************************************************************/


#ifndef SERVERTYPES_H
#define SERVERTYPES_H

#include "localTypes.h"

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
