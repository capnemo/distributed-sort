#include <iostream>
#include "filePartition.h"

void printArgs(const strVec& args);
int main(int argc, char *argv[])
{
    filePartition fP("/work/data/100m", "/tmp/out");
    fP.setInterval(100 * 1024 * 1024);
    strVec iArgs;
    while(fP.getNextIter(iArgs) == true) 
        printArgs(iArgs);
    
    filePartition fP2("/work/common/dataG/1b", "/tmp/out");
    fP2.setNumIter(4);
    strVec slice;
    while(fP2.getNextIter(slice) == true)
        printArgs(slice);
}

void printArgs(const strVec& args)
{
    for (auto mem:args)
        std::cout << mem << " ";
    std::cout << std::endl;
}

