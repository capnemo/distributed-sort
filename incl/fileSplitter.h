#include <cstdint>
#include <fstream>

class fileSplitter {
    
    public:
    fileSplitter(std::string& name):fileName(name) {}

    bool setSegSize(uint64_t size);
    bool setNumSegments(uint32_t num); 
    uint64_t getNextOffset(uint64_t currentOffset);
       
    private:
    bool openFile();
    std::ifstream inFile;
    std::string fileName;
    uint64_t segSize = 0;
    uint32_t numSegs = 0;
    uint64_t fileSize;
};
