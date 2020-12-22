#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "filePartition.h"
#include "blockSortHandler.h"
#include "globalLogger.h"
#include "blockSort.h"
#include "bufferedWriter.h"

static const std::string sortFileStub = "/s";
static const std::string mergeFileStub = "/m";
typedef std::pair<std::string, std::string> strPair;
typedef std::queue<std::string> strQ;

/***************************************************************
FUNCTION: blockSortHandler::runSort
IN inFile        input file to be sorted.
IN outFile       output file.

Calls handler with the appropriate args.
***************************************************************/

bool blockSortHandler::runSort(const std::string& inputFile, 
                               const std::string& outputFile)
{
    strVec sortArgs;
    sortArgs.push_back(inputFile);
    sortArgs.push_back(std::to_string(0));

    struct stat sBuf;
    if (stat(inputFile.c_str(), &sBuf) == -1) {
        globalLogger::logEntry("Error opening input file");
        return false;
    }
    sortArgs.push_back(std::to_string(sBuf.st_size));
    sortArgs.push_back(outputFile);

    return handler(sortArgs);
}

/***************************************************************
FUNCTION: blockSortHandler::handler
IN: args Input arguments. 
arg[0] Input file.
arg[1] Start range in the input file.
arg[2] End range in the input file.
arg[3] ouput file.

Sorts the input range and writes to an output file.
Unlike blockSort::handler, it uses threads directly and does not use 
the thread api. Sorts the blocks and merges without the use of 
intermediate files.
****************************************************************/

bool blockSortHandler::handler(const strVec& args) 
{
    if (args.size() != 4)
        return false;

    std::string inputFile = args[0];
    uint64_t beginRange = std::stoull(args[1]);
    uint64_t endRange = std::stoull(args[2]);
    std::string outputFile = args[3];
    
    uint64_t blockSizeLimit = 512 * 1024 * 1024;
    uint32_t numCores = std::thread::hardware_concurrency() - 1;
    uint32_t blockSize = std::min(blockSizeLimit, 
                                 (endRange - beginRange)/numCores);

    globalLogger::logEntry("Sort block size is " + std::to_string(blockSize));
    filePartition fP(inputFile, "");
    fP.setInterval(blockSize, beginRange, endRange);
    
    globalLogger::logEntry("Starting sort");
    strVec block_args;
    std::vector<blockSort *> sortVec;
    std::vector<std::thread *> threadVec;
    int serial = 0;
    while(fP.getNextIter(block_args) == true) {
        blockSort *bs = new blockSort(block_args, serial++);
        sortVec.push_back(bs);
        std::thread *th = new std::thread(&blockSort::operator(), bs);
        threadVec.push_back(th);
    }

    for (auto m:threadVec)
        m->join();

    std::vector<charPtrVec> blockVec;
    for (auto m:sortVec) {
        charPtrVec recList;
        m->getRecords(recList);
        blockVec.push_back(recList);
    }

    globalLogger::logEntry("Starting merge");
    std::vector<int> indicies(blockVec.size(), 0);
    bufferedWriter mergeWriter(outputFile.c_str(), 10 * 1024 * 1024);
    if (mergeWriter.startBuffers() == false)  {
        globalLogger::logEntry("Merge writer init failed");
        return false;
    }

    while (indicies.size() != 0) {
        int minIndex = 0;
        char* min = blockVec[minIndex][indicies[minIndex]];
        for (int i = 1; i < indicies.size() - 1; i++) {
            if (strcmp(min, blockVec[i][indicies[i]]) > 0) {
                minIndex = i;
                min = blockVec[i][indicies[i]];
            }
        }

        mergeWriter.addToBuffer(min);
        indicies[minIndex]++;
        if (indicies[minIndex] == blockVec[minIndex].size()) {
            indicies.erase(indicies.begin() + minIndex);
            blockVec.erase(blockVec.begin() + minIndex);
        }
    }
    
    for (auto m:sortVec) 
        delete m;

    mergeWriter.stopWrites();
    mergeWriter.cleanup();
    globalLogger::logEntry("merge complete");
    return true;
}
