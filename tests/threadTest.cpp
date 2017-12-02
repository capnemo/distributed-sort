#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "../incl/threadPool.h"

int genRand(const strVec& args);
int main(int argc, char *argv[])
{
    funcMap tab = {{'r', genRand}};
    threadPool tP(4, tab);
    std::vector<struct subTask> rVec;
    std::string segSize = std::to_string(100000);
    std::string fileStub = "/tmp/out.";

    for (uint32_t i = 0; i < 12; i++) {
        std::string id;
        tP.dispatchTask('r', {segSize, fileStub + std::to_string(i)}, id);
    }

    tP.startDispatch();
    strVec fails;
    tP.waitForCompletion(fails);
    if (fails.size() != 0) {
        std::cout << "Error!"  << std::endl;
        for (auto mem:fails)
            std::cout << mem << std::endl;
    }

    tP.terminate();
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
