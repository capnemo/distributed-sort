#include <cstring>
#include <cstdio>
#include <iostream>
#include "globalLogger.h"
#include "bufferedReader.h"


/***************************************************************
FUNCTION: bufferedReader::fillBuffers()
Runs in its own thread. Reads and stuffs buffers.
****************************************************************/

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


/***************************************************************
FUNCTION: bufferedReader::readFromStream
IN: buff
IN: len

Reads to buff from the file.

****************************************************************/

void bufferedReader::readFromStream(char *buff, uint32_t& len)
{
    inFStr.read(buff, bufferSize - nominalRecordSize);
    uint32_t end = inFStr.gcount() - 1;
    while (buff[end] != '\n') 
        buff[++end] = inFStr.get();
    
    len = end;
}

/***************************************************************
FUNCTION: bufferedReader::initBuffers()
Initializes all the data structures.

****************************************************************/
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

/***************************************************************
FUNCTION: bufferedReader::getBuffer()
Preps the current buffer and turns it into a set of records.

****************************************************************/
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


/***************************************************************
FUNCTION: bufferedReader::checkCurrentLine() 
Check if the current buffer is consumed. If so, refresh it.

****************************************************************/
void bufferedReader::checkCurrentLine() 
{
    if (currentLine == lines.size()) 
        getBuffer();
}


/***************************************************************
FUNCTION: bufferedReader::getCurrentLine() 
Return the current line from the current buffer.
****************************************************************/

const char * const bufferedReader::getCurrentLine() 
{
    checkCurrentLine();
    return lines[currentLine];
}

/***************************************************************
FUNCTION: bufferedReader::getLineAndIncr() 
Get the current line and increment the counter.
****************************************************************/

const char * const bufferedReader::getLineAndIncr() 
{
    checkCurrentLine();
    return lines[currentLine++];
}

/***************************************************************
FUNCTION: bufferedReader::returnBuffer()
IN:buff Buffer to be returned.

Recycle a buffer
****************************************************************/

void bufferedReader::returnBuffer(char *buff)
{
    std::lock_guard<std::mutex> rexLock(resMtx);
    reserveList.push_back(buff);
}

/***************************************************************
FUNCTION: bufferedReader::getReserveBuffer()
Returns a buffer from the reserve list. 
****************************************************************/
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

/***************************************************************
FUNCTION: bufferedReader::isReadComplete() 
Returns if the file has been read completely.
****************************************************************/
bool bufferedReader::isReadComplete() 
{
#if 0
    std::cout << std::boolalpha << " " << ioAvail  << " " << bufferAlive
              << " " << currentLine << " " << lines.size() << std::endl;
#endif

    if (ioAvail  || bufferAlive || (currentLine < lines.size()))  
        return false;
 
    if (bufferQ.size() > 0)
        return false;

    return true;
}

/***************************************************************
FUNCTION: bufferedReader::cleanup()
Cleanup prior to shutdown. Shutdown all threads cleanup all data structures.
****************************************************************/
void bufferedReader::cleanup()
{
    readTh->join();

    delete readTh;
    for (auto mem:reserveList) 
        delete [] mem;

    if (currentBuffer != nullptr)
        delete [] currentBuffer;

}
