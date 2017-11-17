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

    dispatch dT(8888, &logSink);
    if (dT.startDispatch() == false) {
        std::cout << "Cannot start the dispatch thread" << std::endl;
        return -1;
    }
    
    uint32_t serNo = 0;
    strVec mergeArgs;
    uint64_t segBegin = 0;
    uint64_t segEnd;
    filePartition fs(inputFile);
    fs.setNumSegments(numAgents);
    while((segEnd = fs.getNextOffset(segBegin)) != 0) {

        std::string sortOut;
        if ((fs.lastSegment() == true) && (serNo == 0))  
            sortOut = outputFile;
        else
            sortOut = outputFile + "." + std::to_string(serNo++);

        mergeArgs.push_back(sortOut);
        strVec args = {inputFile, std::to_string(segBegin), 
                       std::to_string(segEnd), sortOut};
        std::string id;
        dT.dispatchTask('s', args, id);
        segBegin = segEnd;
    }

    bool sortError = false;
    uint32_t numResults = 0;
    while(mergeArgs.size() > numResults) {
        struct result res;
        if (dT.fetchResults(res) == true) {
            numResults++;
            if (res.rc != 0) 
                sortError = true;
        }
    }
    
    if (serNo == 0) {
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
    std::string mergeId;
    dT.dispatchTask('m', mergeArgs, mergeId);

    struct result res;
    while (dT.fetchResults(res) == false);

    if (res.rc != 0) {
        logSink.addEntry("Error during the merge phase");
    } else {
        for (uint32_t i = 0; i < mergeArgs.size() - 1; i++)
            unlink(mergeArgs[i].c_str());
    }

    dT.terminate();
}
