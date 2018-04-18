/***************************************************************************************
FILENAME: threadPool.h

DESCRIPTION:
    Class to start and supervise to completion threads executing subTasks.
    Derived from the dispatch class.

NOTES:
    Cannot be copy constructed and cannot be assigned.
***************************************************************************************/


#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <queue>
#include <map>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "dispatch.h"
#include "worker.h"

typedef std::map<char, int (*)(const strVec&args)> funcMap;

class threadPool:public dispatch {
    public:
    threadPool(funcMap& fT):
    maxThreads(std::thread::hardware_concurrency()), functionTable(fT) {}

    threadPool(uint32_t mT, funcMap& fT):maxThreads(mT), functionTable(fT) {}

    bool startDispatch();
    void dispatchTask(char ty, const strVec& tArgs, std::string& taskId);
    void waitForCompletion(strVec& failedIds);
    void terminate();

    threadPool(const threadPool&) = delete;
    threadPool& operator = (const threadPool&) = delete;
    ~threadPool() = default;

    private:
    void queryThreads();

    private:
    std::queue<struct subTask> sQ;
    std::queue<struct subTask> resQ; //Should become a vector??
    bool workersAlive = true;
    std::vector<worker*> threadVec;
    uint32_t totalIn = 0;
    uint32_t currentId = 0;
    uint32_t maxThreads;
    funcMap functionTable;
    std::thread* quThr = 0;
    std::mutex stMtx;
    std::condition_variable endCond;
};
#endif  /* THREADPOOL_H */
