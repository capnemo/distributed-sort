#include <sstream>
#include <iomanip>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "tcpUtil.h"
#include "serverTypes.h"
#include "taskInitiator.h"
#include "fileUtil.h"
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
    struct stat inStat;
    if (stat(inputFile.c_str(), &inStat) != 0) {
        std::cout << "Error, cannot open input file" << std::endl;
        return -1;
    }
    
    logger logSink;
    if (logSink.startLogger("initiator.log") == false) {
        std::cout << "Cannot start logger" << std::endl;
        return -1;
    }

    int servSocket = tcpUtil::getBoundServerSocket(8888);
    if (servSocket == -1) {
        logSink.addEntry("Cannot get server socket");
        std::cout << "Cannot get server socket" << std::endl;
        return -1;
    }

    dispatch dT(servSocket, &logSink);

    uint64_t inputFileSize = inStat.st_size;
    offPairVec fileSegments;
    uint64_t fragSize = inputFileSize/numAgents;
    fileUtil::splitFileBySize(inputFile, 0, inputFileSize, fragSize, 
                              fileSegments);
    uint32_t serNo = 0;
    strVec mergeArgs;
    for (auto mem:fileSegments) {
        dT.manageQs();
        std::string sortOut = outputFile + "." + std::to_string(serNo++);
        if (fileSegments.size() == 1)
            sortOut = outputFile;
        strVec args = {inputFile, std::to_string(mem.first), 
                       std::to_string(mem.second), sortOut};
        mergeArgs.push_back(sortOut);
        struct task t;
        dT.getNewTask('s', args, t);
        dT.dispatchTask(t);
    }

    bool sortError = false;
    uint32_t numResults = 0;
    while(mergeArgs.size() > numResults) {
        dT.manageQs();
        struct result res;
        if (dT.fetchResults(res) == true) {
            numResults++;
            if (res.rc != 0) 
                sortError = true;
        }
    }
    
    if (fileSegments.size() == 1) {
        dT.terminate();
        return 0;
    }

    if (sortError == true)  
        logSink.addEntry("Error during the sort phase");

    if (numResults <= 1) {
        dT.terminate();
        logSink.addEntry("Not enough files to merge");
        return 0;
    }

    mergeArgs.push_back(outputFile);
    struct task mergeTask;
    dT.getNewTask('m', mergeArgs, mergeTask);
    dT.dispatchTask(mergeTask);
    struct result res;
    while (dT.fetchResults(res) == false)
        dT.manageQs();

    if (res.rc != 0) {
        logSink.addEntry("Error during the merge phase");
    } else {
        for (uint32_t i = 0; i < mergeArgs.size() - 1; i++)
            unlink(mergeArgs[i].c_str());
    }

    dT.terminate();
}
