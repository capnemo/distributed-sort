#include <sstream>
#include <iomanip>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "tcpUtil.h"
#include "serverTypes.h"
#include "taskInitiator.h"
#include "filePartition.h"
#include "nwDispatch.h"
#include "dispatchIterations.h"
#include "globalLogger.h"


int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <inputFile> <outputFile>" << 
                      " <num agents>" << std::endl;
        return -1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    uint32_t numAgents = std::stoi(argv[3]);

    globalLogger::initLogger("logs/initiator.log");
    
    filePartition fP(inputFile, outputFile);
    if (fP.setNumIter(numAgents) == false) {
        globalLogger::logEntry("Cannot open Input file");
        return -1;
    }

    nwDispatch dT(8888);
    if (dT.startDispatch() == false) {
        std::cout << "Cannot start the dispatch thread" << std::endl;
        return -1;
    }
    
    std::cout << "After start dispatch" << std::endl;
    strVec mergeArgs;
    dispatchIters(dT, fP, 's', mergeArgs);
    
    if (mergeArgs.size() <= 1) {
        globalLogger::logEntry("Not enough files to merge or too many sort errors");
        dT.terminate();
        return true;
    }
    
    mergeArgs.push_back(outputFile);
    std::string mergeId;
    dT.dispatchTask('m', mergeArgs, mergeId);
    strVec failedTasks;
    dT.waitForCompletion(failedTasks);

    if (failedTasks.size() == 1) {
        globalLogger::logEntry("Error during the merge phase");
    } else {
        for (uint32_t i = 0; i < mergeArgs.size() - 1; i++)
            unlink(mergeArgs[i].c_str());
    }

    dT.terminate();
 
#if 0
    std::map<std::string, std::string> mergeTable;
    strVec slice;
    while(fP.getNextIter(slice) != false) {
        std::string id;
        dT.dispatchTask('s', slice, id);
        mergeTable.insert({id, slice[slice.size() - 1]});
    }

    strVec failedTasks;
    dT.waitForCompletion(failedTasks);
    if (failedTasks.size() != 0) {
        std::string logLine = "Failed sorts";
        for (auto mem:failedTasks) {
            logLine += " " + mem;
            mergeTable.erase(mem);
        }
        logSink.addEntry(logLine);
    } else 
        logSink.addEntry("Sort successful");
    
    if (mergeTable.size() == 1) {
        logSink.addEntry("Not enough files to merge");
        return 0;
    }

    strVec mergeArgs;
    for (auto mem:mergeTable)
        mergeArgs.push_back(mem.second);
    mergeArgs.push_back(outputFile);
    std::string mergeId;
    dT.dispatchTask('m', mergeArgs, mergeId);
    dT.waitForCompletion(failedTasks);

    if (failedTasks.size() == 1) {
        logSink.addEntry("Error during the merge phase");
    } else {
        for (uint32_t i = 0; i < mergeArgs.size() - 1; i++)
            unlink(mergeArgs[i].c_str());
    }

    dT.terminate();
#endif
}
