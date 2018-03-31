#ifndef SORTHANDLER_H
#define SORTHANDLER_H

#include <queue>

#include "localTypes.h"
#include "msgHandler.h"
#include "threadPool.h"

typedef std::vector<std::pair<std::string, std::string>> strPairVec;

class sortHandler:public msgHandlerBase {
    public:
    sortHandler(){}
    virtual bool handler(const strVec& args);

    private:
    void printToLog(const std::string prefix, char type, 
                    const struct subTask& sT);

    void printToLog(const std::string prefix, char type, uint32_t id, 
                    const strVec& args);

    void reduceFileQ(std::queue<std::string>& fileQ, strPairVec& filePairs);
    void addMergeTaskToPool(std::string& m1, std::string& m2);
};

#endif /*SORTHANDLER_H*/
