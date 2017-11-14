#include "bufferedReader.h"
#include "bufferedWriter.h"
#include <cstring>
#include <iostream>
#include <ctime>

typedef std::vector<std::string> strVec;
void printTime();
int mergeFiles(const strVec& args);
static const uint32_t writeBufferSize = 10 * 1024 * 1024;
static const uint32_t readBufferSize = 10 * 1024 * 1024;
/*
int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cout << "usage: " << argv[0] << " inputFile1" << " inputFile2"
                     " outputFile" << std::endl;
        return -1;
    }

    strVec inArgs;
    for(int i = 1; i < argc; i++) 
        inArgs.push_back(std::string(argv[i]));
    
    mergeFiles(inArgs);

    return 0;
}
*/
int mergeFiles(const strVec& args)
{
    
    bufferedReader inStr1(args[0].c_str(), readBufferSize);
    if (inStr1.initBuffers() == false) 
        return -1;
    
    bufferedReader inStr2(args[1].c_str(), readBufferSize);
    if (inStr2.initBuffers() == false) 
        return -1;

    bufferedWriter outStr(args[2].c_str(), writeBufferSize);
    if (outStr.startBuffers() == false) 
        return -1;
    

    while(!inStr1.isReadComplete() && !inStr2.isReadComplete()) {
        int cmp = strcmp(inStr1.getCurrentLine(), inStr2.getCurrentLine());
        if (cmp < 0) {
            outStr.addToBuffer(inStr1.getLineAndIncr());
        } else if (cmp > 0) {
            outStr.addToBuffer(inStr2.getLineAndIncr());
        } else {
            outStr.addToBuffer(inStr1.getLineAndIncr());
            outStr.addToBuffer(inStr2.getLineAndIncr());
        }
    }
    
    while(!inStr1.isReadComplete()) 
        outStr.addToBuffer(inStr1.getLineAndIncr());

    while(!inStr2.isReadComplete()) 
        outStr.addToBuffer(inStr2.getLineAndIncr());
    
    inStr1.cleanup();
    inStr2.cleanup();
    
    outStr.stopWrites();
    outStr.cleanup();

    return 0;
}

void printTime()
{
    std::cout << time(0) << std::endl;
}
