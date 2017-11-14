#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "../incl/threadPool.h"

int genRand(const strVec& args);
int main(int argc, char *argv[])
{
    threadPool tP(4);
    tP.startWorkers();
    std::vector<struct subTask> rVec;
    std::string segSize = std::to_string(100000);
    std::string fileStub = "/tmp/out.";

    for (uint32_t i = 0; i < 12; i++) {
        struct subTask s = {i, genRand, {segSize, 
                                         fileStub + std::to_string(i)}, 10};
        tP.addToPool(s);
    }

    while(rVec.size() < 12)
        tP.getTaskResults(rVec);

    tP.stopAllThreads();
   //strVec fArgs = {"100", "/tmp/out"};
   //genRand(fArgs);
}


int genRand(const strVec& args) 
{
    uint64_t numRecs = std::stoull(args[0]);
    std::ofstream outStr(args[1]);
    if (outStr.is_open() == false)
        return -1;

    for(uint64_t i = 0; i < numRecs; i++) {
        uint64_t rNum = random()%1000000;
        std::stringstream ss;
        ss << std::setw(6) << std::setfill('0') << random() % 1000000;
        outStr << ss.str() << std::endl;
    }
    return 0;
}
