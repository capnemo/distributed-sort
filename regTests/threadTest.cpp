#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "../incl/threadPool.h"

int genRand(const strVec& args);
int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cout << "usage: " << "<output file name> <file size> " 
                  << "<number of files>" << std::endl;
        return -1;
    }

    std::string fileStub = argv[1];
    std::string segSize = argv[2];
    uint32_t numFiles = std::stoi(argv[3]);

    funcMap tab = {{'r', genRand}};
    threadPool tP(4, tab);
    std::vector<struct subTask> rVec;

    std::string id;
    for (uint32_t i = 0; i < numFiles; i++) 
        tP.dispatchTask('r', {segSize, fileStub + std::to_string(i)}, id);

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
        std::stringstream ss;
        ss << std::setw(6) << std::setfill('0') << random() % 1000000;
        outStr << ss.str() << std::endl;
    }
    return 0;
}
