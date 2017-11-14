#include "logger.h"
#include "config.h"
#include "localTypes.h"

#ifndef MSGHANDLERBASE_H
#define MSGHANDLERBASE_H
class msgHandlerBase {
    public:
    static msgHandlerBase* getHandler(char handlerId, mrConfig* cfg, 
                                      logger* logS);

    msgHandlerBase(mrConfig* config, logger* log):cfg(config), logSink(log) {}
    
    virtual bool handler(const strVec& args) = 0;

    protected:
    mrConfig* cfg;
    logger* logSink;
};
#endif /*MSGHANDLERBASE_H*/
