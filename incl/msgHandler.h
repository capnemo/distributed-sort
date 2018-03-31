#ifndef MSGHANDLERBASE_H
#define MSGHANDLERBASE_H

#include "logger.h"
#include "localTypes.h"

class msgHandlerBase {
    public:
    msgHandlerBase(){}
    static msgHandlerBase* getHandler(char handlerId);
    
    virtual bool handler(const strVec& args) = 0;
    virtual ~msgHandlerBase() = default;

};
#endif /*MSGHANDLERBASE_H*/
