#include <queue>
#include <mutex>
#include <thread>

#include "worker.h"

#ifndef THREADPOOL_H
#define THREADPOOL_H
class threadPool {
    public:
    threadPool():maxThreads(std::thread::hardware_concurrency()/2) {}
    threadPool(uint32_t mT):maxThreads(mT) {}
    void startWorkers();
    void addToPool(struct subTask& tsk);
    void getTaskResults(taskVec& res);
    void stopAllThreads();
    
    private:
    void queryThreads();

    private:
    std::queue<struct subTask> sQ;
    std::queue<struct subTask> resQ;
    std::mutex stMtx;
    std::mutex rMtx;
    std::thread* quThr = 0;
    bool workersAlive = true;
    std::vector<worker*> threadVec;
    uint32_t maxThreads;
};
#endif  /* THREADPOOL_H */
