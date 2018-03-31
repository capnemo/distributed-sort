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
    void decodeTask(const std::string& pStr, task& tsk);
    void decodeResult(const std::string& rStr, struct result& res);
    void encodeResult(const struct result& res, std::string& rStr);
    void getTerminateMsg(std::string& msg);
    bool endOfConnection(const std::string& msg);
}
#endif /*PROTOCOL_H*/
