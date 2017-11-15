#include <iostream>
#include "fileSplitter.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <file name> <part size>" 
                  << std::endl;
        return -1;
    }

    std::string inputFile = argv[1];
    fileSplitter fs(inputFile);
    if (fs.setNumSegments(std::stoi(argv[2])) == false) {
        std::cout << "Error.." << std::endl;
        return -1;
    }

    uint64_t segBegin = 0;
    uint64_t segEnd;
    while((segEnd = fs.getNextOffset(segBegin)) != 0)  {
        std::cout << segBegin << " " << segEnd << std::endl;
        segBegin = segEnd;
    }
    
}

