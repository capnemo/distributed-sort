#include "msgHandler.h"
#include "sortHandler.h"
#include "multiMergeHandler.h"

/***************************************************************
FUNCTION: msgHandlerBase::getHandler
IN: handlerId Id of the requested handler.

Returns a pointer to the handler.
****************************************************************/

msgHandlerBase* msgHandlerBase::getHandler(char handlerId) 
{

    switch (handlerId) {
        case 's':
            return new sortHandler();
        case 'm':
            return new multiMergeHandler();
        default:
            return nullptr;
    }
}

