/***************************************************************************************
FILENAME: brTest.cpp

DESCRIPTION: 
    Sanity test for bufferedReader. Driver will have to compare the input and output files.

NOTES:

***************************************************************************************/


#include <iostream>
#include "bufferedReader.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <input file name>" << std::endl;
        return -1;
    }
    
    bufferedReader br(argv[1], 10 * 1024 * 1024);
    br.initBuffers();
    std::ofstream outStr(std::string(argv[1]) + ".out");

    while(br.isReadComplete() == false) 
        outStr << br.getLineAndIncr() << std::endl;

    br.cleanup();
}
