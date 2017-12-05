#include <queue>
#include <map>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "worker.h"

#ifndef THREADPOOL_H
#define THREADPOOL_H

typedef std::map<char, int (*)(const strVec&args)> funcMap;

class threadPool {
    public:
    threadPool(funcMap& fT):
    maxThreads(std::thread::hardware_concurrency()/2), functionTable(fT) {}

    threadPool(uint32_t mT, funcMap& fT):maxThreads(mT), functionTable(fT) {}

    void startDispatch();
    void dispatchTask(char ty, const strVec& tArgs, std::string& taskId);
    void waitForCompletion(strVec& failedIds);
    void terminate();

    /*
    void startWorkers();
    void addToPool(struct subTask& tsk);
    void getTaskResults(taskVec& res);
    void stopAllThreads();
    */
    
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
    //std::mutex rMtx;
};
#endif  /* THREADPOOL_H */
