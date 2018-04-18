#include "bufferedWriter.h"
#include "globalLogger.h"
#include <string.h>
#include <iostream>

/***************************************************************
FUNCTION: bufferedWriter::writeBuffers() 
Write the current buffer to file. Runs in its own independent thread
****************************************************************/

void bufferedWriter::writeBuffers() 
{
    while (incoming) {
        std::unique_lock<std::mutex> qLck(qMtx);
        if (writeQ.size() == 0) 
            condVar.wait(qLck);

        ptrSizePair qTop(nullptr, 0);
        if (writeQ.size() != 0) {
            qTop = writeQ.front();
            writeQ.pop();
        }
    
        qLck.unlock();
        if (qTop.first != nullptr) {
            outFStr.write(qTop.first, qTop.second);
            returnBuffer(qTop.first);
        }
    }
}

/***************************************************************
FUNCTION: bufferedWriter::returnBuffer
IN: retBuff Buffer to be recycled.

Recycles the buffer.
****************************************************************/

void bufferedWriter::returnBuffer(char *retBuff)
{
    std::lock_guard<std::mutex> resLock(resMtx);
    reserveList.push_back(retBuff);

}

/***************************************************************
FUNCTION: bufferedWriter::startBuffers()

Starts the write thread and sets up the data structures.
****************************************************************/

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


/***************************************************************
FUNCTION: bufferedWriter::getNewBuffer() 
Gets a new buffers from the reserve list. If the list is empty, 
it does a new.
****************************************************************/
char* bufferedWriter::getNewBuffer() 
{
    char *newBuff = nullptr;
    resMtx.lock();
    if (reserveList.size() != 0) {
        newBuff = reserveList.front();
        reserveList.pop_front();
    }
    resMtx.unlock();

    if (newBuff == nullptr) 
        newBuff = new char[bufferSize];

    return newBuff;
}

/***************************************************************
FUNCTION: bufferedWriter::cycleBuffers
IN: newBuff Whether a new buffer is required.

Switches the current buffer to a new one. Used when the current buffer
is full.
****************************************************************/
void bufferedWriter::cycleBuffers(bool newBuff)
{
    std::unique_lock<std::mutex> qLck(qMtx);
    writeQ.push(ptrSizePair(currentBuffer, currentPos));
    condVar.notify_one();
    qLck.unlock();

    if (newBuff == true)
        currentBuffer = getNewBuffer();
    else
        currentBuffer = nullptr;
    currentPos = 0;
}

/***************************************************************
FUNCTION: bufferedWriter::addToBuffer
IN: line Record to be added to the current buffer
IN: lineSize Record size.

Add a record to the current buffer
****************************************************************/


void bufferedWriter::addToBuffer(const char *line, uint32_t lineSize)
{
    if (lineSize + currentPos  + 1 > bufferSize)
        cycleBuffers(true);
    
    memcpy(currentBuffer + currentPos, line, lineSize);
    currentPos += lineSize;
    currentBuffer[currentPos++] = '\n';
}


/***************************************************************
FUNCTION: bufferedWriter::stopWrites()
Stop the write thread and prep for shutdown.

****************************************************************/
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

/***************************************************************
FUNCTION: bufferedWriter::cleanup()
Cleanup all data structures.
    


****************************************************************/
void bufferedWriter::cleanup()
{
    delete writeTh;

    if (currentBuffer != nullptr)
        delete currentBuffer;

    while (reserveList.size() != 0) {
        char* t = reserveList.front();
        delete [] t;
        reserveList.pop_front();
    }
}
