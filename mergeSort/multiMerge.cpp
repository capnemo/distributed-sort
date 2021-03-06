#include <string>
#include <iostream>
#include "localTypes.h"
#include "bufferedWriter.h"
#include "bufferedReader.h"
#include "multiMergeHandler.h"
#include "globalLogger.h"

/***************************************************************
FUNCTION: multiMerge(
IN: args
args[0..(size - 2)] input files.
args[size - 1] merged output file.

Merges multiple files using buffered I/O
****************************************************************/

bool multiMerge(const strVec& args) 
{
    uint32_t buffSz = 10 * 1024 * 1024;
    std::vector<bufferedReader*> readers;
    
    if (args.size() < 3) 
        return false;

    bufferedWriter writer(args[args.size() - 1].c_str(), buffSz);
    if (writer.startBuffers() == false) {
        writer.stopWrites();
        writer.cleanup();
        return false;
    }
    
    bool rdFail = false;
    for (int i = 0; i < args.size() - 1; i++) {
        bufferedReader* bR = new bufferedReader(args[i].c_str(), buffSz);
        if (bR->initBuffers() == false) {
            globalLogger::logEntry("Reader failed to open " + args[i]);
            rdFail = true;
            break;
        }
        readers.push_back(bR);
    }

    if (rdFail == true) {
        for (auto mem:readers) {
            mem->cleanup();
            delete mem;
        }
        writer.stopWrites();
        writer.cleanup();
        return false;
    }

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
    return true;
}

#if 0
void multiMergeHandler::printToLog(const std::string line)
{
    logSink->addEntry(line);
}
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
