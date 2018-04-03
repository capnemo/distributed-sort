#ifndef BUFFEREDREADER_H
#define BUFFEREDREADER_H

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
    const char * const getCurrentLine();
    bool isReadComplete();
    void cleanup();
    bufferedReader(const bufferedReader&) = delete;
    bufferedReader& operator = (const bufferedReader&) = delete;
    ~bufferedReader() = default;
    
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

    std::mutex qMtx;
    std::queue<ptrSizePair> bufferQ;
    std::mutex resMtx;
    std::list<char *> reserveList;
    std::vector<char *> lines;
    bool bufferAlive = false;
    std::thread *readTh;
    std::condition_variable condVar;
};

#endif /* BUFFEREDREADER_H */
