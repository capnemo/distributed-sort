#include <iostream>
#include "threadPool.h"
#include "mergeSort.h"

void threadPool::dispatchTask(char ty, const strVec& taskArgs, 
                              std::string& tId)
{
    tId = std::to_string(currentId++);
    struct subTask st = {tId, 0, taskArgs, 10};
    st.func = functionTable.find(ty)->second;

    std::lock_guard<std::mutex> lck(stMtx);
    totalIn++;
    if (st.func != 0) {
        sQ.push(st);
    } else {
        st.result = 1;
        resQ.push(st);
    }
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
                std::lock_guard<std::mutex> lck(stMtx);
                resQ.push(r);
                if (resQ.size() == totalIn)
                    endCond.notify_one();
            }
        }
    }
}

void threadPool::startDispatch()
{
    for (uint32_t i = 0; i < maxThreads; i++)
        threadVec.push_back(new worker());

    quThr = new std::thread(&threadPool::queryThreads, this);
}

void threadPool::waitForCompletion(strVec& failedIds)
{
    std::unique_lock<std::mutex> lck(stMtx);
    while(totalIn > resQ.size())
        endCond.wait(lck);

    while (resQ.size() != 0) {
        subTask r = resQ.front();
        if (r.result != 0)
            failedIds.push_back(r.id);
        resQ.pop();
    }
}

void threadPool::terminate()
{
    workersAlive = false;
    quThr->join();
    for (auto mem:threadVec) {
        mem->terminate();
        delete mem;
    }
}
