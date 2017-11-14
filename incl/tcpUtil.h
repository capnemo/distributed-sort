#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>

#ifndef TCPUTIL_H
#define TCPUTIL_H

namespace tcpUtil {
    const uint32_t msgSizeLimit = 256;
    int getBoundServerSocket(uint16_t port);
    int getConnectedClientSocket(const char* servIp, uint16_t servPort);
    void prepareAddressStruct(struct sockaddr_in* servAddr, 
                              in_addr_t servIp, uint16_t port);
    void writeWithSizePrefix(int fd, const std::string& buff);
    void writeWithSizePrefix(int fd, const char *buffer, uint16_t sz);
    void readFromSocket(int fd, std::string& msg);
    void readFromSocket(int fd, char *buffer, uint16_t& buffSz);
}

#endif /*TCPUTIL_H*/

