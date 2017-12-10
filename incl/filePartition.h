#include <cstdint>
#include <fstream>
#include <vector>
#include <string>

#include "iterGen.h"

#ifndef FILEPARTITION_H
#define FILEPARTITION_H

typedef std::vector<std::string> strVec;
typedef std::pair<uint64_t, uint64_t> offPair;

class filePartition: public iterGen {
    
    public:
    filePartition(const std::string inFile, const std::string outFile): 
                fileName(inFile), outStub(outFile) {}

    bool setInterval(uint64_t inter, uint64_t begin = 0, uint64_t end = 0);
    bool setNumIter(int32_t num);
    bool getNextIter(strVec& iterArgs);
    //bool getNextIter(offPair& iter);
    
    private:
    void openFile();
    void getArgs(uint64_t off1, uint64_t off2, strVec& fullArgs);

    private:
    std::string fileName;
    std::string outStub;
    std::ifstream inStr;
    uint32_t serial = 0;
    bool fileOpen = false;
    uint64_t fileBegin;
    uint64_t fileEnd;
    int32_t numIntervals = -1;
    int64_t interval = -1;
    uint64_t currentOffset = 0;
    uint64_t fileSize = 0;
};

#endif /* FILEPARTITION_H */
