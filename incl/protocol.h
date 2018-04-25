/***************************************************************************************
FILENAME: protocol.h

DESCRIPTION:
    Namespace for functions that implement a request response protocol for initiator agent
    communications. 

NOTES:

***************************************************************************************/


#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include "localTypes.h"
#include "serverTypes.h"
namespace protocol {
    void writeResult(int sock, struct result& rst);
    bool readTask(int sock, struct task& tsk);
    bool readResult(int sock, struct result& rst);
    void writeTask(int sock, struct task& tsk);
    void terminateClient(int sock);
    void splitString(const std::string& inStr, strVec& fragments);
    void encodeTask(const task& tsk, std::string& pStr);
    bool decodeTask(const std::string& pStr, task& tsk);
    bool decodeResult(const std::string& rStr, struct result& res);
    void encodeResult(const struct result& res, std::string& rStr);
    void getTerminateMsg(std::string& msg);
    void getTaskId(uint32_t taskSerial, std::string& id);
    bool endOfConnection(const std::string& msg);
}
#endif /*PROTOCOL_H*/
