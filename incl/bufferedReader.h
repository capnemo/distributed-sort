#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <list>
#include <cstdint>


class bufferedReader {
    public:
    bufferedReader(const char *fName, uint32_t buffSz) :fileName(fName),
                                                      bufferSize(buffSz) {}
    bool initBuffers();
    const char * const getLineAndIncr();
    void getLine(char *lineBuff, uint32_t size);
    const char * const getCurrentLine();
    //const char * const getNextLine();
    bool isReadComplete();
    void cleanup();
    
    private:
    void fillBuffers();
    void getBuffer();
    void returnBuffer(char *buff);
    char* getReserveBuffer();
    void checkCurrentLine();
    void readFromStream(char *buff, uint32_t& len);

    private:
    std::string fileName;
    uint32_t bufferSize;
    std::ifstream inFStr;
    bool ioAvail;
    const uint32_t bufferSizeLimit = 10;
    const uint32_t nominalRecordSize = 20;
    uint16_t reserveSize = 10;
    uint32_t currentLine = 0;
    char* currentBuffer = 0;
    typedef std::pair<char *, uint32_t> ptrSizePair;
    std::queue<ptrSizePair> bufferQ;
    std::list<char *> reserveList;
    std::vector<char *> lines;
    bool bufferAlive = false;
    std::thread *readTh;
    std::mutex resMtx;
    std::mutex qMtx;
    std::condition_variable condVar;
};
