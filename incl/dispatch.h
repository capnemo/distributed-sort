#include <queue>
#include <map>
#include <ctime>
#include <poll.h>

#include "localTypes.h"
#include "serverTypes.h"
#include "logger.h"

#ifndef DISPATCH_H
#define DISPATCH_H

class dispatch {
    
    public:
    dispatch(int s, logger* logS):servSock(s), logSink(logS) {}
    void manageQs();
    void dispatchTask(task& newTask);
    bool fetchResults(result& rc);
    void terminate();
    void getNewTask(char tType, const strVec& tArgs, struct task& newTask);

    private:
    void lookForNewClients();
    void handleReads();
    void handleWrites();
    void logTask(const std::string& prefix, const struct task& tsk);
    void logResult(const std::string& prefix, const struct result& rslt);
    void getTaskId(std::string& id);
    int getOldestClient();
    
    private:
    struct cliState {
        cliState(int s):fd(s),active(false),lastReply(0) {}
        int fd;
        bool active;
        time_t lastReply;
    };

    int servSock;
    const int16_t readMask = POLLIN;
    const int16_t writeMask = POLLOUT;
    std::map<int, cliState> clientList;
    std::queue<task>  taskQ;
    std::queue<result>  resultQ;
    logger* logSink;
    uint32_t taskId = 0;
};

#endif /*DISPATCH_H*/
