#include <queue>
#include <map>
#include <ctime>
#include <poll.h>
#include <thread>
#include <mutex>

#include "localTypes.h"
#include "serverTypes.h"
#include "logger.h"

#ifndef DISPATCH_H
#define DISPATCH_H

class dispatch {
    
    public:
    dispatch(int p, logger* logS):port(p), logSink(logS) {}
    bool startDispatch();
    void dispatchTask(task& newTask);
    void dispatchTask(char ty, const strVec& tArgs, std::string& taskId);
    bool fetchResults(result& rc);
    void terminate();
    void getNewTask(char tType, const strVec& tArgs, struct task& newTask);

    private:
    void manageQs();
    void lookForNewClients();
    void handleReads();
    void handleWrites();
    void logTask(const std::string& prefix, const struct task& tsk);
    void logResult(const std::string& prefix, const struct result& rslt);
    void getTaskId(std::string& id);
    int getOldestClient();
    void addToResults(struct result& rsl);
    
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
    std::queue<result>  resultQ;
    logger* logSink;
    uint32_t taskId = 0;
    bool alive = true;

    std::thread* dispatchThr;
    std::mutex disMtx;
};

#endif /*DISPATCH_H*/
