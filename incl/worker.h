/***************************************************************************************
FILENAME: worker.h

DESCRIPTION:
    Class to run a subTask in an independent thread. 
    Supervised by an instance of threadPool.

NOTES:
    Cannot be copy constructed and cannot be assigned.
***************************************************************************************/


#ifndef WORKER_H
#define WORKER_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "localTypes.h"

class worker {
    public:
    worker() {}

    void addTask(subTask& nextTask);
    bool getResult(struct subTask& res);
    bool ready(); 
    void terminate();
    worker(const worker&) = delete;
    worker& operator = (const worker&) = delete;
    ~worker() = default;

    private:
    void threadFunc();

    private:
    std::queue<struct subTask> taskQ;
    std::queue<struct subTask> resultsQ;
    bool finish = false;
    std::thread* exTh = 0;
    std::mutex runMtx;
    std::condition_variable condVar;
    const uint32_t maxInQ = 1;

};
#endif /* WORKER_H */
