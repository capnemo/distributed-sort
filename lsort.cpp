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
}
