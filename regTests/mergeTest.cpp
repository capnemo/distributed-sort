/***************************************************************************************
FILENAME: mergeTest.cpp

DESCRIPTION:
    Test to merge multiple sorted files into one file. Driver will only check the sizes 
    of the input and output files.

NOTES:

***************************************************************************************/


#include <iostream>
#include "multiMergeHandler.h"

int main(int argc, char *argv[])
{
    if (argc < 4) { 
        std::cout << "usage: " << argv[0] << "<input files..> <output file>"
                  << std::endl;
        return -1;
    }

    strVec mergeArgs(argv + 1, argv + argc);
    multiMergeHandler mm;
    if (mm.handler(mergeArgs) != true) 
        std::cout << "Merge failed" << std::endl;
}
