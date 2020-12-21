/***************************************************************************************
FILENAME: msgHandler.h

DESCRIPTION:
    Abstract base class for the agent side functions.

NOTES:

***************************************************************************************/


#ifndef MSGHANDLERBASE_H
#define MSGHANDLERBASE_H

#include "logger.h"
#include "localTypes.h"

class msgHandlerBase {
    public:
    msgHandlerBase() {}
    virtual bool handler(const strVec& args) = 0;
    static msgHandlerBase* getHandler(char handlerId);
    virtual ~msgHandlerBase() = default;
};
#endif /*MSGHANDLERBASE_H*/
