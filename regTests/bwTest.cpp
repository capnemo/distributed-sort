#include <iostream>
#include "bufferedWriter.h"
int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <input file name>" << std::endl;
        return -1;
    }
        
    std::string wFile = std::string(argv[1]) + ".out";
    bufferedWriter bw(wFile.c_str(), 10 * 1024 * 1024);
    bw.startBuffers();
    std::ifstream outStr(argv[1]);

    char line[128];
    while (outStr) {
        outStr.getline(line, 128);
        if (outStr.gcount() != 0)
            bw.addToBuffer(line);
    }
    
    bw.stopWrites();
    bw.cleanup();
}
