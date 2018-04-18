/***************************************************************************************
FILENAME: bufferedWriter.h

DESCRIPTION:
    Class to write large file with delimited records
    Records are written to the object one at a time and written to the file one block at a time.
    Buffers are recycled. All writes happen in an independent thread

NOTES:
    Class is not copy constructible or assignable. It is also not to be inherited.

***************************************************************************************/



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
    bufferedWriter(const bufferedWriter&) = delete;
    bufferedWriter& operator = (const bufferedWriter&) = delete;
    ~bufferedWriter() = default;

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
