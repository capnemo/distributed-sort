#include <iostream>
#include <map>
#include "localTypes.h"
#include "serverTypes.h"
#include "protocol.h"
#include "config.h"
#include "logger.h"
#include "msgHandler.h"
#include "tcpUtil.h"
//Port in the config file. 
//config file on the command line ??
void encodeRC(std::string& tId, bool result, std::string& reply);

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " ip" << std::endl;
        return -1;
    }
    mrConfig config("config.txt");
    config.loadTable();

    char hostname[32];
    std::string localName;
    if (gethostname(hostname, 32) != 0)
        localName = "noname";
    else 
        localName = hostname;

    std::string logFileName;
    mrConfig::getLogFileName(config, localName, logFileName);

    logger logSink;
    logSink.startLogger(logFileName);

    int sock;
    if ((sock = tcpUtil::getConnectedClientSocket(argv[1], 8888)) == -1) {
        std::cout << "Error getting a socket to the server!" << std::endl;
        return -1;
    }
 
    bool done = false;
    while (done == false) {   //Redo the loop.
        std::string msg;
        tcpUtil::readFromSocket(sock, msg);
        if (msg.size() == 0) {
            logSink.addEntry("NW Error!");
            break;
        }

        if (protocol::endOfConnection(msg) == true) {
            logSink.addEntry("End of session");
            break;
        }
    
        logSink.addEntry(msg);
        struct task remTask;
        protocol::decodeTask(msg, remTask);
        msgHandlerBase* mH = msgHandlerBase::getHandler(remTask.type, &config, 
                                                    &logSink);
        
        if (mH == 0) {
            logSink.addEntry("Error! Bad Task Id");
            break;
        }
        
        bool rc = false;
        if (mH != 0) 
            rc = mH->handler(remTask.args);

        int16_t handRc = (rc) ? 0:1;
        struct result res = {remTask.id, handRc};
        protocol::writeResult(sock, res);
        logSink.addEntry(remTask.id + " " + std::to_string(handRc));
    }
}
