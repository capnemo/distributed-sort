#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "filePartition.h"


/***************************************************************
FUNCTION: filePartition::getNextIter()
OUT: iterArgs Arguments of the iteration in a string vector

Generate an iteration with each call. 
****************************************************************/

bool filePartition::getNextIter(strVec& iterArgs)
{
    if (fileOpen == false)
        return fileOpen;

    if (currentOffset >= fileEnd)
        return false;

    uint64_t endOffset = currentOffset + interval - 1;
    if (endOffset > fileEnd) {
        getArgs(currentOffset, fileEnd, iterArgs);
        currentOffset = fileEnd;
        return true;
    }

    int c;
    inStr.seekg(endOffset);
    while((c = inStr.get()) != '\n');

    endOffset = inStr.tellg();
    getArgs(currentOffset, endOffset, iterArgs);

    currentOffset = endOffset;
    return true;
}

/***************************************************************
FUNCTION: filePartition::getArgs
IN: off1 file offset1
IN: off2 file offset2
OUT: strVec& fullArgs Formatted arguments

Format arguments of the current iteration
****************************************************************/

void filePartition::getArgs(uint64_t off1, uint64_t off2, strVec& fullArgs)
{
    strVec args = {fileName, std::to_string(off1), std::to_string(off2),
                   outStub + "." + std::to_string(serial++)};

    fullArgs = args;
}

/***************************************************************
FUNCTION: filePartition::openFile()

Open input file.

****************************************************************/

void filePartition::openFile()
{
    struct stat inStat;
    if (stat(fileName.c_str(), &inStat) != 0) 
        return;
    fileSize = inStat.st_size;                       

    inStr.open(fileName);
    fileOpen = inStr.is_open();
}

/***************************************************************
FUNCTION: filePartition::setInterval
IN: inter Interval for each iteration
IN: begin Beginning of the segment in the file.
IN: end End of the segment in the file.

Sets the interval for each iteration.
****************************************************************/

bool filePartition::setInterval(uint64_t inter, uint64_t begin, uint64_t end)
{
    openFile();
    if (fileOpen == false)
        return false;
    
    interval = inter;
    fileBegin = begin;
    currentOffset = begin;
    if (end == 0) 
        fileEnd = fileSize;
    else 
        fileEnd = end;

    return true;
}

/***************************************************************
FUNCTION: filePartition::setNumIter()
IN: num Number of iterations.

Sets the number of iterations.
****************************************************************/

bool filePartition::setNumIter(int32_t num)
{
    openFile();
    if (fileOpen == false)
        return false;
 
    numIntervals = num;
    interval = fileSize/numIntervals;
    fileBegin = 0;
    currentOffset = 0;
    fileEnd = fileSize;   

    return true;
}
