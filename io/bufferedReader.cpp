#include <cstring>
#include <cstdio>
#include <iostream>
#include "globalLogger.h"
#include "bufferedReader.h"

void bufferedReader::fillBuffers()
{
    while (inFStr) {

        std::unique_lock<std::mutex> qLck(qMtx);
        if (bufferQ.size() >= bufferSizeLimit) 
            condVar.wait(qLck);
        uint32_t bSz = bufferQ.size();
        qLck.unlock();

        uint32_t size = 0;
        char *eBuff;
        if (bSz < bufferSizeLimit)  {
            eBuff = getReserveBuffer();
            readFromStream(eBuff, size);
        } else 
            continue;

        if (size != 0) {
            qLck.lock();
            bufferQ.push({eBuff, size});
            qLck.unlock();
            eBuff = nullptr;
        } else
            returnBuffer(eBuff);
    }
    ioAvail = false;
}

#if 0
void bufferedReader::fillBuffers()
{
    char* eBuff = 0;

    while (inFStr) {
        if (eBuff == 0)
            eBuff = getReserveBuffer();

        std::unique_lock<std::mutex> qLck(qMtx);
        if (bufferQ.size() >= bufferSizeLimit) 
            condVar.wait(qLck);
        uint32_t bSz = bufferQ.size();
        qLck.unlock();

        uint32_t size = 0;
        if (bSz < bufferSizeLimit) 
            readFromStream(eBuff, size);

        if (size != 0) {
            qLck.lock();
            bufferQ.push({eBuff, size});
            qLck.unlock();
            eBuff = 0;
        }
    }
    ioAvail = false;
}

#endif

void bufferedReader::readFromStream(char *buff, uint32_t& len)
{
    inFStr.read(buff, bufferSize - nominalRecordSize);
    uint32_t end = inFStr.gcount() - 1;
    while (buff[end] != '\n') 
        buff[++end] = inFStr.get();
    
    len = end;
}

bool bufferedReader::initBuffers()
{   
    inFStr.open(fileName);
    ioAvail = inFStr.is_open();

    if (!ioAvail) {
        globalLogger::logEntry("Error opening file " + fileName);
        return ioAvail;
    }
    
    for (uint16_t i = 0; i < reserveSize; i++)  
        reserveList.push_back(new char[bufferSize]);
    
    readTh = new std::thread(&bufferedReader::fillBuffers, this);
    return ioAvail;
}

void bufferedReader::getBuffer()
{
    std::unique_lock<std::mutex> qLck(qMtx);
    if (bufferQ.size() < bufferSizeLimit) 
        condVar.notify_one();
    uint32_t bSz = bufferQ.size();
    qLck.unlock();
    if (bSz == 0) {
        bool isEmpty = true;
        while (isEmpty == true) {
            qLck.lock();
            isEmpty = (bufferQ.size() == 0) ? true : false;
            qLck.unlock();
        }
    }
    
    qLck.lock();
    ptrSizePair b = bufferQ.front();
    bufferQ.pop();
    bufferAlive = (bufferQ.size() > 0) ? true:false;
    qLck.unlock();

    if (currentBuffer != nullptr)
        returnBuffer(currentBuffer);
    
    currentBuffer = b.first;
    uint32_t sz = b.second;
    currentLine = 0;
    lines.clear();
    currentBuffer[sz] = '\0';
    lines.push_back(currentBuffer);
    for(uint32_t i = 0; i < sz; i++) {
        if (currentBuffer[i] == '\n') {
            currentBuffer[i] = '\0';
            lines.push_back(currentBuffer + i + 1);
        }
    }
}

void bufferedReader::checkCurrentLine() 
{
    if (currentLine == lines.size()) 
        getBuffer();
}

const char * const bufferedReader::getCurrentLine() 
{
    checkCurrentLine();
    return lines[currentLine];
}

const char * const bufferedReader::getLineAndIncr() 
{
    checkCurrentLine();
    return lines[currentLine++];
}

void bufferedReader::returnBuffer(char *buff)
{
    std::lock_guard<std::mutex> rexLock(resMtx);
    reserveList.push_back(buff);
}

char* bufferedReader::getReserveBuffer()
{
    char *rBuff = nullptr;
    
    resMtx.lock();
    if (reserveList.size() > 0) {
        rBuff = reserveList.front();
        reserveList.pop_front();
    }
    resMtx.unlock();

    if (rBuff == nullptr) 
        rBuff = new char[bufferSize];

    return rBuff;
}

bool bufferedReader::isReadComplete() 
{
    /*
    std::cout << std::boolalpha << " " << ioAvail  << " " << bufferAlive
              << " " << currentLine << " " << lines.size() << std::endl;
    */
    if (ioAvail  || bufferAlive || (currentLine < lines.size()))  
        return false;
 
    if (bufferQ.size() > 0)
        return false;

    return true;
}

void bufferedReader::cleanup()
{
    readTh->join();

    delete readTh;
    for (auto mem:reserveList) 
        delete [] mem;

    if (currentBuffer != nullptr)
        delete [] currentBuffer;

}
