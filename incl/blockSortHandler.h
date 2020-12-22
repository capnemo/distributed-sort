/***************************************************************************************
FILENAME: blockSortHandler.h

DESCRIPTION:
    Handler to sort a multiple blocks from a single large file. 

NOTES:
    
***************************************************************************************/


#ifndef BLOCKSORTHANDLER_H
#define BLOCKSORTHANDLER_H

#include "localTypes.h"
#include "msgHandler.h"

class blockSortHandler:public msgHandlerBase {
    public:
    blockSortHandler() {}
    bool runSort(const std::string& inputFile, const std::string& outputFile);

    virtual bool handler(const strVec& args);
    blockSortHandler(const blockSortHandler&) = delete;
    blockSortHandler& operator = (const blockSortHandler&) = delete;
    virtual ~blockSortHandler() = default;
};

#endif /*BLOCKSORTHANDLER_H*/
