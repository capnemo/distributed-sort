#include "logger.h"
#include "config.h"
#include "localTypes.h"
#include "msgHandler.h"

#ifndef MULTIMERGEHANDLER_H
#define MULTIMERGEHANDLER_H

class multiMergeHandler:public msgHandlerBase {
    public:
    multiMergeHandler(mrConfig* config, logger* log):msgHandlerBase(config,log) {}
    virtual bool handler(const strVec& args);

    void printToLog(const std::string line);
};

#endif /*MULTIMERGEHANDLER_H*/
