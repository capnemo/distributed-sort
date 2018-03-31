#ifndef MULTIMERGEHANDLER_H
#define MULTIMERGEHANDLER_H

#include "localTypes.h"
#include "msgHandler.h"

class multiMergeHandler:public msgHandlerBase {
    public:
    multiMergeHandler(){}
    virtual bool handler(const strVec& args);

    void printToLog(const std::string line);
};

#endif /*MULTIMERGEHANDLER_H*/
