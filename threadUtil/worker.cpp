#include <iostream>
#include <cstdint>
#include <ctime>
#include "worker.h"


/***************************************************************
FUNCTION: worker::addTask
IN: nextTask Task to be scheduled.

Schedules a task to be run.
****************************************************************/

void worker::addTask(subTask& nextTask)
{
    if (exTh == nullptr) 
        exTh = new std::thread(&worker::threadFunc, this);

    std::unique_lock<std::mutex> lck(runMtx);
    taskQ.push(nextTask);
    condVar.notify_one();
}

/***************************************************************
FUNCTION: worker::ready()

Returns if we are ready to accept a new task.
****************************************************************/

bool worker::ready()
{
    std::lock_guard<std::mutex> lck(runMtx);
    return (taskQ.size() < maxInQ) ? true:false;
}


/***************************************************************
FUNCTION: worker::threadFunc()

Executes the task. Runs in its own thread.
****************************************************************/
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

/***************************************************************
FUNCTION: worker::terminate()

Terminates the execution thread and deletes all data structures.
****************************************************************/

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

/***************************************************************
FUNCTION: worker::getResult
OUT: res

Returns the result of a task.
****************************************************************/
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
