#include "msgHandler.h"
#include "sortHandler.h"
#include "multiMergeHandler.h"

msgHandlerBase* msgHandlerBase::getHandler(char handlerId, 
                                       mrConfig* cfg, logger* logS) {
    switch (handlerId) {
        case 's':
            return new sortHandler(cfg, logS);
        case 'm':
            return new multiMergeHandler(cfg, logS);
        default:
            return 0;
    }
}

