#include <iostream>
#include "threadPool.h"

void threadPool::addToPool(struct subTask& tsk) 
{
    std::lock_guard<std::mutex> lck(stMtx);
    sQ.push(tsk);
}

void threadPool::queryThreads() 
{
    while (workersAlive == true) {
        for (auto mem:threadVec) {
            std::lock_guard<std::mutex> lck(stMtx);
            if ((mem->ready() == true) && (sQ.size() > 0)){
                subTask sT = sQ.front();
                mem->addTask(sT);
                sQ.pop();
            }
        }

        for (auto mem:threadVec) {
            struct subTask r;
            bool avail = mem->getResult(r);
            if (avail == true) {
                std::lock_guard<std::mutex> lck(rMtx);
                resQ.push(r);
            }
        }
    }
}

void threadPool::startWorkers()
{
    for (uint32_t i = 0; i < maxThreads; i++)
        threadVec.push_back(new worker());

    quThr = new std::thread(&threadPool::queryThreads, this);
}

void threadPool::getTaskResults(taskVec& results)
{
    std::lock_guard<std::mutex> lck(rMtx);
    while (resQ.size() != 0) {
        subTask r = resQ.front();
        results.push_back(r);
        resQ.pop();
    }
}

void threadPool::stopAllThreads()
{
    workersAlive = false;
    quThr->join();
    for (auto mem:threadVec) {
        mem->terminate();
        delete mem;
    }
}
