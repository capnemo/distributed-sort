#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "fileSplitter.h"


bool fileSplitter::setSegSize(uint64_t size)
{
    segSize = size;
    return openFile();
}

bool fileSplitter::setNumSegments(uint32_t num) 
{
    struct stat inStat;
    if (stat(fileName.c_str(), &inStat) != 0) 
        return false;
    
    fileSize = inStat.st_size;
    segSize = inStat.st_size/num;
    return openFile();
}

uint64_t fileSplitter::getNextOffset(uint64_t currentOffset)
{
    if (currentOffset == fileSize)
        return 0;

    inFile.seekg(currentOffset + segSize - 1);
    int c;
    while((c = inFile.get()) != '\n');
    return inFile.tellg();
}
   
bool fileSplitter::openFile()
{
    inFile.open(fileName);
    return inFile.is_open();
}

