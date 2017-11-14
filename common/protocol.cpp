#include "protocol.h"
#include "tcpUtil.h"

//REPLACE LITERALS
//All decode functions should return bool.

static const char* termMsg = "F";

void protocol::writeResult(int sock, struct result& rst)
{
    std::string rStr;
    encodeResult(rst, rStr);
    tcpUtil::writeWithSizePrefix(sock, rStr);
}

bool protocol::readTask(int sock, struct task& tsk) 
{
    std::string tStr = "";
    tcpUtil::readFromSocket(sock, tStr);
    if (tStr == "")
        return false;
    
    protocol::decodeTask(tStr, tsk);

    return true;
}

bool protocol::readResult(int sock, struct result& rst)
{
    std::string rStr = "";
    tcpUtil::readFromSocket(sock, rStr);
    if (rStr == "")
        return false;

    rst.rc = 100;
    decodeResult(rStr, rst);
    if (rst.rc == 100)
        return false;

    return true;
}

void protocol::writeTask(int sock, struct task& tsk)
{
    std::string tStr;
    encodeTask(tsk, tStr);
    tcpUtil::writeWithSizePrefix(sock, tStr);
}

void protocol::terminateClient(int sock)
{
    tcpUtil::writeWithSizePrefix(sock, termMsg);
}

void protocol::splitString(const std::string& str, strVec& fragments)
{
    uint32_t begin = 0;
   
    for(uint32_t i = 0; i < str.size(); i++) {
        if (str[i] == ' ') {
            fragments.push_back(str.substr(begin, i - begin));
            begin = i + 1;
        }
    }
    fragments.push_back(str.substr(begin));
}

void protocol::encodeTask(const task& tsk, std::string& pStr)
{
    pStr = tsk.id + " " + tsk.type;
    for(auto mem:tsk.args)
        pStr += " " + mem;
}

void protocol::decodeTask(const std::string& pStr, task& tsk)
{
    strVec frags;
    splitString(pStr, frags);
    
    if (frags.size() <= 2)
        return;

    tsk.id = frags[0];
    tsk.type = frags[1][0];
    for (uint32_t i = 2; i < frags.size(); i++)
        tsk.args.push_back(frags[i]);
}

void protocol::decodeResult(const std::string& rStr, struct result& res)
{
    strVec frags;
    splitString(rStr, frags);

    if (frags.size() != 2)
        return;

    res.id = frags[0];
    res.rc = std::stoi(frags[1]);
}

void protocol::encodeResult(const struct result& res, std::string& rStr)
{
    rStr = res.id + " ";
    rStr += std::to_string(res.rc);
}

//NO LITERALS IN THE MIDDLE OF THE CODE!!!!
void protocol::getTerminateMsg(std::string& msg) 
{
    msg = "F";
}

bool protocol::endOfConnection(const std::string& msg) 
{
    return (msg[0] == 'F') ? true:false;
}
