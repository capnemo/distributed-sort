#include "bufferedWriter.h"
#include "globalLogger.h"
#include <string.h>
#include <iostream>

void bufferedWriter::writeBuffers() 
{
    while (incoming) {
        std::unique_lock<std::mutex> qLck(qMtx);
        if (writeQ.size() == 0) 
            condVar.wait(qLck);

        ptrSizePair qTop(0, 0);
        if (writeQ.size() != 0) {
            qTop = writeQ.front();
            writeQ.pop();
        }
    
        qLck.unlock();
        if (qTop.first != 0) {
            outFStr.write(qTop.first, qTop.second);
            returnBuffer(qTop.first);
        }
    }
}

void bufferedWriter::returnBuffer(char *retBuff)
{
    std::lock_guard<std::mutex> resLock(resMtx);
    reserveList.push_back(retBuff);

}

bool bufferedWriter::startBuffers()
{
    outFStr.open(fileName);
    if (outFStr.is_open() == false) {
        globalLogger::logEntry("Error Opening File " + fileName);
        return false;
    }

    for(uint32_t i = 0; i <= reserveSize; i++)   
        reserveList.push_back(new char[bufferSize]);

    currentBuffer = reserveList.front();
    reserveList.pop_front();

    incoming = true;   
    writeTh = new std::thread(&bufferedWriter::writeBuffers, this);
    return true;
}

char* bufferedWriter::getNewBuffer() 
{
    char *newBuff = 0;
    resMtx.lock();
    if (reserveList.size() != 0) {
        newBuff = reserveList.front();
        reserveList.pop_front();
    }
    resMtx.unlock();

    if (newBuff == 0) 
        newBuff = new char[bufferSize];

    return newBuff;
}

void bufferedWriter::cycleBuffers(bool newBuff)
{
    std::unique_lock<std::mutex> qLck(qMtx);
    writeQ.push(ptrSizePair(currentBuffer, currentPos));
    condVar.notify_one();
    qLck.unlock();

    if (newBuff == true)
        currentBuffer = getNewBuffer();
    else
        currentBuffer = 0;
    currentPos = 0;
}

void bufferedWriter::addToBuffer(const char *line, uint32_t lineSize)
{
    if (lineSize + currentPos  + 1 > bufferSize)
        cycleBuffers(true);
    
    memcpy(currentBuffer + currentPos, line, lineSize);
    currentPos += lineSize;
    currentBuffer[currentPos++] = '\n';
}

void bufferedWriter::stopWrites()
{
    cycleBuffers(false);
    incoming = false;

    std::unique_lock<std::mutex> qLck(qMtx);
    condVar.notify_one();
    qLck.unlock();

    writeTh->join();
    
    //This loop should execute in the thread and not outside
    while (writeQ.size() != 0) {
        ptrSizePair p = writeQ.front();
        outFStr.write(p.first, p.second);
        writeQ.pop();
        returnBuffer(p.first);
    }
}

void bufferedWriter::cleanup()
{
    delete writeTh;

    if (currentBuffer != 0)
        delete currentBuffer;

    while (reserveList.size() != 0) {
        char* t = reserveList.front();
        delete [] t;
        reserveList.pop_front();
    }
}
