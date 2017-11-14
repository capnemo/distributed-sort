#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mergeSort.h"
#include "bufferedReader.h"
#include "bufferedWriter.h"

namespace mergeSort {
struct charPtrCmp {
    bool operator() (const char *s1, const char* s2) {
        return (strcmp(s1,s2) < 0) ? true: false;
    }
};

void writeMergeFile(std::ofstream& str, char *rec);

int sortBlock(const strVec& args)
{
    
    std::ifstream inFileStr(args[0]);
    std::ofstream outFileStr(args[3]);

    if ((inFileStr.is_open() == false) || (outFileStr.is_open() == false)) {
        std::cout << "SORT OUT!!!!" << std::endl;

        return -1;
    }

    uint64_t beginRange = std::stoll(args[1]);
    uint64_t endRange = std::stoll(args[2]);
    uint64_t buffSz = endRange - beginRange;
    char* buffer = new char[buffSz];


    inFileStr.seekg(beginRange);
    inFileStr.read(buffer, buffSz);
    
    std::vector<char *> outVec;
    uint32_t pos = 0;
    outVec.push_back(buffer + pos);

    for(pos = 0; pos < buffSz - 1; pos++) {
        if (buffer[pos] == '\n') {
            buffer[pos] = '\0';
            outVec.push_back(buffer + pos + 1);
        }
    }
    buffer[buffSz - 1] = '\0';

    charPtrCmp comp;
    std::sort(outVec.begin(), outVec.end(), comp);

    for(auto mem:outVec) {
        outFileStr.write(mem, strlen(mem));
        outFileStr.write("\n", strlen("\n"));
    }
    
    delete [] buffer;
    return 0;
}

int mergeBlocks(const strVec& args)
{
    const char *inFile1 = args[0].c_str();
    const char *inFile2 = args[1].c_str();
    const char *outFile= args[2].c_str();

    std::ifstream inStream1(inFile1);
    std::ifstream inStream2(inFile2);
    std::ofstream outStream(outFile);

    if (!inStream1 || !inStream2 || !outStream) 
        return -1;

    uint16_t recSz = 128;
    char rec1[128], rec2[128];

    inStream1.getline(rec1, recSz);
    inStream2.getline(rec2, recSz);
    while (inStream1 && inStream2) {
        
        if ((rec1[0] == '\0') && (rec2[0] != '\0')) 
            writeMergeFile(outStream, rec2);
        else if ((rec1[0] != '\0') && (rec1[0] == '\0'))
            writeMergeFile(outStream, rec1);

        int cmp = strcmp(rec1, rec2);
        if (cmp < 0) {
            writeMergeFile(outStream, rec1);
            inStream1.getline(rec1, recSz);
        } else if (cmp > 0) {
            writeMergeFile(outStream, rec2);
            inStream2.getline(rec2, recSz);
        } else {
            writeMergeFile(outStream, rec1);
            writeMergeFile(outStream, rec2);
            inStream1.getline(rec1, recSz);
            inStream2.getline(rec2, recSz);
        }
    }

    while(inStream1) {
        writeMergeFile(outStream, rec1);
        inStream1.getline(rec1, recSz);
    }

    while(inStream2) {
        writeMergeFile(outStream, rec2);
        inStream2.getline(rec2, recSz);
    }

    return 0;
}

void writeMergeFile(std::ofstream& str, char *rec)
{
    strcat(rec, "\n");
    str.write(rec, strlen(rec));
}

int mergeFiles(const strVec& args)
{
    const uint32_t writeBufferSize = 10 * 1024 * 1024;
    const uint32_t readBufferSize = 10 * 1024 * 1024;
    
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
}
