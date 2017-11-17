#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "filePartition.h"


bool filePartition::setSegSize(uint64_t size)
{
    segSize = size;
    return openFile();
}

bool filePartition::setNumSegments(uint32_t num) 
{
    struct stat inStat;
    if (stat(fileName.c_str(), &inStat) != 0) 
        return false;
    
    fileSize = inStat.st_size;
    segSize = inStat.st_size/num;
    return openFile();
}

uint64_t filePartition::getNextOffset(uint64_t currentOffset)
{
    if (currentOffset == fileSize)
        return 0;

    uint64_t partEnd = currentOffset + segSize - 1;
    if (partEnd > fileSize)
        partEnd = fileSize - 1;

    inFile.seekg(partEnd);
    int c;
    while((c = inFile.get()) != '\n');

    uint64_t pEnd = inFile.tellg();
    endOfFile = (pEnd == fileSize) ? true : false;
    return pEnd;
}
 
bool filePartition::lastSegment()
{
    return endOfFile;
}
   
bool filePartition::openFile()
{
    inFile.open(fileName);
    return inFile.is_open();
}

