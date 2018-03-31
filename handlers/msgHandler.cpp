#include "msgHandler.h"
#include "sortHandler.h"
#include "multiMergeHandler.h"

msgHandlerBase* msgHandlerBase::getHandler(char handlerId) {
    switch (handlerId) {
        case 's':
            return new sortHandler();
        case 'm':
            return new multiMergeHandler();
        default:
            return nullptr;
    }
}

