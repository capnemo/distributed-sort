/***************************************************************************************
FILENAME: dispatch.h

DESCRIPTION:
    Abstract class for nwDispatch and threadPool. 
    Sets the interface for "fan out" from one task to a number of subtasks. 
NOTES:
    Abstract class.

***************************************************************************************/


#ifndef DISPATCH_H
#define DISPATCH_H

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
