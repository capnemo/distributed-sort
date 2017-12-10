#include <iostream>
#include <fstream>
#include <queue>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "threadPool.h"
#include "config.h"
#include "mergeSort.h"
#include "sortMerge.h"
#include "filePartition.h"
#include "sortHandler.h"
#include "dispatchIterations.h"
#include "globalLogger.h"
#include "globalConfig.h"

static const std::string sortFileStub = "/s";
static const std::string mergeFileStub = "/m";
typedef std::pair<std::string, std::string> strPair;
typedef std::queue<std::string> strQ;

bool sortHandler::handler(const strVec& args) 
{
    if (args.size() != 4)
        return false;

    std::string inputFile = args[0];
    uint64_t beginRange = std::stoull(args[1]);
    uint64_t endRange = std::stoull(args[2]);
    std::string outputFile = args[3];
    
    std::string scratchDir;
    globalConfig::getValueByKey("scratchLocation", scratchDir);
    uint64_t blockSize;
    globalConfig::getValueByKey("sortBlockSize", blockSize);

    if ((blockSize == 0) || (scratchDir == "")) {
        globalLogger::logEntry("Config File Invalid");
        return false;
    }
    blockSize *= 1024 * 1024;

    filePartition fP(inputFile, scratchDir + sortFileStub);
    fP.setInterval(blockSize, beginRange, endRange);

    funcMap fTab = {{'s', mergeSort::sortBlock}};
    threadPool tP(8, fTab);
    tP.startDispatch();

    strVec mergeArgs;
    dispatchIters(tP, fP, 's', mergeArgs);
    if (mergeArgs.size() <= 1)  {
        globalLogger::logEntry("Not enough files to merge or too many sort errors");
        tP.terminate();
        return false;
    }
    
    mergeArgs.push_back(outputFile);
    if (multiMerge(mergeArgs) == false)
        globalLogger::logEntry("Merge Failed");
    
    for (uint32_t i = 0; i < mergeArgs.size() - 1; i++)
        unlink(mergeArgs[i].c_str());

    tP.terminate();
    return true;
}
