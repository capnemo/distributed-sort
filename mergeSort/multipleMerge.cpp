#include <string>
#include <iostream>
#include "localTypes.h"
#include "bufferedWriter.h"
#include "bufferedReader.h"

/*
    
    !!!!!!!!!!!!!!!!!!!!!! DEPRICATED !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !!!!!!!!!!!!!!!!!!!!!! DEPRICATED !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !!!!!!!!!!!!!!!!!!!!!! DEPRICATED !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !!!!!!!!!!!!!!!!!!!!!! DEPRICATED !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    DO NOT USE.
*/
int mergeMultipleFiles(const strVec& args) 
{
    if (args.size() < 3) 
        return false;

    uint32_t buffSz = 10 * 1024 * 1024;
    std::vector<bufferedReader*> readers;
    
    for (int i = 0; i < args.size() - 1; i++) {
        bufferedReader* bR = new bufferedReader(args[i].c_str(), buffSz);
        bR->initBuffers();
        readers.push_back(bR);
    }

    bufferedWriter writer(args[args.size() - 1].c_str(), buffSz);
    writer.startBuffers();
    
    while (readers.size() > 0) {

        const char* currMin = readers[0]->getCurrentLine();
        uint32_t minRdr = 0;
        for (uint32_t i = 0; i < readers.size(); i++) {
            if (readers[i] != nullptr) {
                if (strcmp(currMin, readers[i]->getCurrentLine()) > 0) {
                    currMin = readers[i]->getCurrentLine();
                    minRdr = i;
                }
            }
        }

        writer.addToBuffer(readers[minRdr]->getLineAndIncr());

        if (readers[minRdr]->isReadComplete() == true) {
            readers[minRdr]->cleanup();
            delete readers[minRdr];
            readers.erase(readers.begin() + minRdr);
        }
    }
    
    writer.stopWrites();
    writer.cleanup();
    return 0;
}

#if 0
int main(int argc, char *argv[])
{
    if (argc < 4)  {
        std::cout << "Error!" << std::endl;
        return -1;
    }
    strVec inArgs;
    for(uint32_t i = 1; i < argc; i++) 
        inArgs.push_back(argv[i]);

    mergeMultipleFiles(inArgs);
}
#endif
