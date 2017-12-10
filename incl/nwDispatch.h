#include <queue>
#include <map>
#include <ctime>
#include <poll.h>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "dispatch.h"
#include "localTypes.h"
#include "serverTypes.h"

#ifndef NWDISPATCH_H
#define NWDISPATCH_H

class nwDispatch :public dispatch {
    
    public:
    nwDispatch(int p):port(p){}
    bool startDispatch();
    void dispatchTask(char ty, const strVec& tArgs, std::string& taskId);
    void waitForCompletion(strVec& failedIds);
    void terminate();

    private:
    void manageQs();
    void lookForNewClients();
    void handleReads();
    void handleWrites();
    void dispatchTask(task& newTask);
    void logTask(const std::string& prefix, const struct task& tsk);
    void logResult(const std::string& prefix, const struct result& rslt);
    void getTaskId(std::string& id);
    int getOldestClient();
    void addToResults(struct result& rsl);
    void getNewTask(char tType, const strVec& tArgs, struct task& newTask);
    
    private:
    struct cliState {
        cliState(int s):fd(s) {}
        int fd;
        bool active = false;
        time_t lastDispatch = 0;
        time_t lastReply = 0;
    };

    int port;
    int servSock;
    const int16_t readMask = POLLIN;
    const int16_t writeMask = POLLOUT;
    std::map<int, cliState> clientList;
    std::queue<task>  taskQ;
    std::vector<result> resultList;
    uint32_t taskId = 0;
    bool alive = true;
    uint32_t tasksDispatched = 0;
    int32_t outstandingTasks = 0;

    std::thread* dispatchThr;
    std::mutex disMtx;
    std::condition_variable endCond;
};

#endif /*NWDISPATCH_H*/
