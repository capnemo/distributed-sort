#include <iostream>
#include <cstdint>
#include "worker.h"

void worker::addTask(subTask& nextTask)
{
    if (exTh == 0) 
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
    bool alive = true;
    while(alive == true) {
        struct subTask newTask = {0, 0, {}, -1};
        std::unique_lock<std::mutex> lck(runMtx);
        if (taskQ.size()  == 0) 
            condVar.wait(lck);
        if (taskQ.size() != 0) {
            newTask = taskQ.front();
            taskQ.pop();
        }
        lck.unlock();
        if (newTask.func != 0) {
            newTask.result = newTask.func(newTask.args);
            std::lock_guard<std::mutex> resLck(resMtx);
            resultsQ.push(newTask);
        }
        lck.lock();
        alive = (taskQ.size() > 0) || (!finish);
    }

}

void worker::terminate()
{
    if (exTh == 0)
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
    std::lock_guard<std::mutex> resLck(resMtx);
    if (resultsQ.size() != 0) {
        res = resultsQ.front();
        resultsQ.pop();
        return true;
    }
    
    return false;
}
