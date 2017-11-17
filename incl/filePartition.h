#include <cstdint>
#include <fstream>

class filePartition {
    
    public:
    filePartition(std::string& name):fileName(name) {}

    bool setSegSize(uint64_t size);
    bool setNumSegments(uint32_t num); 
    uint64_t getNextOffset(uint64_t currentOffset);
    bool lastSegment();
       
    private:
    bool openFile();

    private:
    bool endOfFile;
    std::ifstream inFile;
    std::string fileName;
    uint64_t segSize = 0;
    uint64_t fileSize;
};
