#include <iostream>
#include <libgen.h>
#include "globalLogger.h"
#include "blockSortHandler.h"

void usage(const char* progName);

int main(int argc, char *argv[])
{
    if (argc != 3) {
        usage(argv[0]);
        return -1;
    }

    std::string inFile(argv[1]);
    std::string outFile(argv[2]);
    char *logDir = dirname(argv[2]);
    std::string logFileName = std::string(logDir) + "/ssort.log";
    globalLogger::initLogger(logFileName);

    blockSortHandler blockSorter;
    if (blockSorter.runSort(inFile, outFile) == false) {
        std::cout << "Sort Failed" << std::endl;
        return -1;
    }

    return 0;
}

void usage(const char* progName)
{
    std::cout << "usage: " << progName << " <input file> " 
              << "<output file>" << std::endl;
}

