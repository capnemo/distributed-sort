#include <iostream>
#include <cstdint>
#include <ctime>
#include "worker.h"

void worker::addTask(subTask& nextTask)
{
    if (exTh == nullptr) 
        exTh = new std::thread(&worker::threadFunc, this);

    std::unique_lock<std::mutex> lck(runMtx);
    taskQ.push(nextTask);
    condVar.notify_one();
}


bool worker::ready()
{
    std::lock_guard<std::mutex> lck(runMtx);
    return (taskQ.size() < maxInQ) ? true:false;
}

void worker::threadFunc()
{
    uint32_t tSz = 0;
    while((tSz > 0) || (!finish)) {
        struct subTask newTask = {"", nullptr, {}, -1};
        std::unique_lock<std::mutex> lck(runMtx);
        if (taskQ.size()  == 0) 
            condVar.wait(lck);
        if (taskQ.size() != 0) {
            newTask = taskQ.front();
            taskQ.pop();
        }
        tSz = taskQ.size();
        lck.unlock();
        if (newTask.func != nullptr) {
            newTask.startTime = time(nullptr);
            newTask.result = newTask.func(newTask.args);
            newTask.endTime = time(nullptr);
            std::lock_guard<std::mutex> resLck(runMtx);
            resultsQ.push(newTask);
        }
    }

}

void worker::terminate()
{
    if (exTh == nullptr)
        return;

    finish = true;
    std::unique_lock<std::mutex> lck(runMtx);
    condVar.notify_one();
    lck.unlock();

    exTh->join();
    delete exTh;
}

bool worker::getResult(struct subTask& res)
{
    std::lock_guard<std::mutex> resLck(runMtx);
    if (resultsQ.size() != 0) {
        res = resultsQ.front();
        resultsQ.pop();
        return true;
    }
    return false;
}
