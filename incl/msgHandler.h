#include "logger.h"
#include "localTypes.h"

#ifndef MSGHANDLERBASE_H
#define MSGHANDLERBASE_H
class msgHandlerBase {
    public:
    msgHandlerBase(){}
    static msgHandlerBase* getHandler(char handlerId);
    
    virtual bool handler(const strVec& args) = 0;

};
#endif /*MSGHANDLERBASE_H*/
