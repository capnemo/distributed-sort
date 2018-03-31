
#ifndef BUFFEREDWRITER_H
#define BUFFEREDWRITER_H

#include <queue>
#include <list>
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>
#include <thread>
#include <condition_variable>

class bufferedWriter {

    public:
    bufferedWriter(const char *fName, uint32_t buffSz):fileName(fName),
                                                       bufferSize(buffSz) {}
    bool startBuffers();
    void addToBuffer(const char *line, uint32_t size);
    void addToBuffer(const char *line) { addToBuffer(line, strlen(line));}
    void stopWrites();
    void cleanup();

    private:
    char* getNewBuffer();
    void cycleBuffers(bool newBuff);
    void writeBuffers();
    void returnBuffer(char *retBuff);

    private:
    std::string fileName;
    uint32_t bufferSize;
    uint16_t reserveSize = 10;
    char* currentBuffer;
    uint32_t currentPos = 0;
    bool incoming = false;
    std::ofstream outFStr;
    std::thread* writeTh;
    typedef std::pair<char *, uint32_t> ptrSizePair;
    std::queue<ptrSizePair> writeQ;
    std::list<char *> reserveList;
    std::mutex resMtx;
    std::mutex qMtx;
    std::condition_variable condVar;
};
#endif /* BUFFEREDWRITER_H */
