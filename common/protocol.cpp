#include "protocol.h"
#include "tcpUtil.h"

static const char* termMsg = "F";


/***************************************************************
FUNCTION: protocol::writeResult
IN: sock Socket
IN: rst   result to be written

Write the result to the socket
****************************************************************/

void protocol::writeResult(int sock, struct result& rst)
{
    std::string rStr;
    encodeResult(rst, rStr);
    tcpUtil::writeWithSizePrefix(sock, rStr);
}

/***************************************************************
FUNCTION: protocol:readTask:
IN: sock Socket
OUT: tsk  Task struct to be read

Read a task from the socket.
****************************************************************/
bool protocol::readTask(int sock, struct task& tsk) 
{
    std::string tStr = "";
    tcpUtil::readFromSocket(sock, tStr);
    if (tStr == "")
        return false;
    
    protocol::decodeTask(tStr, tsk);

    return true;
}

/***************************************************************
FUNCTION: protocol::readResult
IN: sock Socket
OUT: rst result struct.

Read a result from the socket
****************************************************************/

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

/***************************************************************
FUNCTION: protocol::writeTask
IN: sock
OUT: tsk

Write task to socket
****************************************************************/

void protocol::writeTask(int sock, struct task& tsk)
{
    std::string tStr;
    encodeTask(tsk, tStr);
    tcpUtil::writeWithSizePrefix(sock, tStr);
}

/***************************************************************
FUNCTION: protocol::terminateClient(int sock)
IN: sock Socket

Terminate connection on the socket.
****************************************************************/
void protocol::terminateClient(int sock)
{
    tcpUtil::writeWithSizePrefix(sock, termMsg);
}

/***************************************************************
FUNCTION: protocol::splitString
IN: str Input string
OUT: fragments output fragments

Split the input string into substrings. Delimiter is ' '.

****************************************************************/
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

/***************************************************************
FUNCTION: protocol::encodeTask
IN: tsk task to be encoded
OUT: pStr String to be encoded to.

Encode a task to a string.

****************************************************************/

void protocol::encodeTask(const task& tsk, std::string& pStr)
{
    pStr = tsk.id + " " + tsk.type;
    for(auto mem:tsk.args)
        pStr += " " + mem;
}

/***************************************************************
FUNCTION: protocol::decodeTask
IN: pStr 
OUT: task

Decode a string to task.

****************************************************************/
bool protocol::decodeTask(const std::string& pStr, task& tsk)
{
    strVec frags;
    splitString(pStr, frags);
    
    if (frags.size() <= 2)
        return false;

    tsk.id = frags[0];
    tsk.type = frags[1][0];
    for (uint32_t i = 2; i < frags.size(); i++)
        tsk.args.push_back(frags[i]);

    return true;
}

/***************************************************************
FUNCTION: protocol::decodeResult
IN: rStr
OUT: res

Decode a string to result.
****************************************************************/
bool protocol::decodeResult(const std::string& rStr, struct result& res)
{
    strVec frags;
    splitString(rStr, frags);

    if (frags.size() != 2)
        return false;

    res.id = frags[0];
    res.rc = std::stoi(frags[1]);

    return true;
}

/***************************************************************
FUNCTION: protocol::encodeResult
IN: res Input result
OUT: rStr Output string.

Encode result to string

****************************************************************/
void protocol::encodeResult(const struct result& res, std::string& rStr)
{
    rStr = res.id + " ";
    rStr += std::to_string(res.rc);
}

/***************************************************************
FUNCTION: protocol::getTerminateMsg
OUT: msg Will contain the terminate message upon return

Return the terminate message.
****************************************************************/

void protocol::getTerminateMsg(std::string& msg) 
{
    msg = termMsg;
}

/***************************************************************
FUNCTION: protocol::endOfConnection
IN: msg Message
Detect end of connection.
****************************************************************/

bool protocol::endOfConnection(const std::string& msg) 
{
    //return (msg[0] == 'F') ? true:false;
    return (msg[0] == termMsg[0]) ? true:false;
}
