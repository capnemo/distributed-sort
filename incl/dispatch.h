#ifndef DISPATCH_H
#define DISPATCH_H

#include <queue>
#include <map>
#include <ctime>
#include <poll.h>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "localTypes.h"
#include "serverTypes.h"

class dispatch {
    public:
    virtual bool startDispatch() = 0;
    virtual void dispatchTask(char ty, const strVec& tArgs, 
                              std::string& taskId) = 0;
    virtual void waitForCompletion(strVec& failedIds) = 0;
    virtual void terminate() = 0;
};
#endif /*DISPATCH_H*/
