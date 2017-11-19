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
#include "dispatch.h"


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
    logger logSink;
    if (logSink.startLogger("logs/initiator.log") == false) {
        std::cout << "Cannot start logger" << std::endl;
        return -1;
    }

    filePartition fs(inputFile);
    if (fs.setNumSegments(numAgents) == false) {
        logSink.addEntry("Cannot open input file");
        return -1;
    }

    dispatch dT(8888, &logSink);
    if (dT.startDispatch() == false) {
        std::cout << "Cannot start the dispatch thread" << std::endl;
        return -1;
    }
    
    uint32_t serNo = 0;
    uint64_t segBegin = 0;
    uint64_t segEnd;
    std::map<std::string, std::string> mergeTable;
    while((segEnd = fs.getNextOffset(segBegin)) != 0) {
        std::string sortOut;
        if ((fs.lastSegment() == true) && (serNo == 0))  
            sortOut = outputFile;
        else
            sortOut = outputFile + "." + std::to_string(serNo++);

        strVec args = {inputFile, std::to_string(segBegin), 
                       std::to_string(segEnd), sortOut};
        std::string id;
        dT.dispatchTask('s', args, id);
        mergeTable.insert({id, sortOut});
        segBegin = segEnd;
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
}
