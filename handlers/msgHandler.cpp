#include "msgHandler.h"
#include "sortHandler.h"
#include "blockSortHandler.h"
#include "multiMergeHandler.h"

/***************************************************************
FUNCTION: msgHandlerBase::getHandler
IN: handlerId Id of the requested handler.

Returns a pointer to the handler.
****************************************************************/

msgHandlerBase* msgHandlerBase::getHandler(char handlerId) 
{

    switch (handlerId) {
        case 'g':
            return new blockSortHandler();
        case 's':
            return new sortHandler();
        case 'm':
            return new multiMergeHandler();
        default:
            return nullptr;
    }
}

