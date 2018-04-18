#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "tcpUtil.h"

/***************************************************************
FUNCTION: tcpUtil::getBoundServerSocket
IN: port port to be bound to.

Create a socket and bind the port to it.
****************************************************************/

int tcpUtil::getBoundServerSocket(uint16_t port)
{
    int sock  = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        return sock;

    int optval;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
        return -1;

    struct sockaddr_in servAddr;
    memset((void *)&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = htons(INADDR_ANY);

    int rc = bind(sock, (const struct sockaddr *)&servAddr,
                         sizeof(servAddr));
    if (rc != 0)
        return -1;

    if (listen(sock, 5) == -1) 
        return -1;

    return sock;
}

/***************************************************************
FUNCTION: tcpUtil::prepareAddressStruct
OUT: servAddr  address struct.
IN: ip IP address
IN: port Port

Convenience function to fill an address struct.
****************************************************************/
void tcpUtil::prepareAddressStruct(struct sockaddr_in* servAddr, in_addr_t ip,
                         uint16_t port)
{
    bzero(servAddr, sizeof(struct sockaddr_in));
    servAddr->sin_family = AF_INET;
    servAddr->sin_port = htons(port);
    servAddr->sin_addr.s_addr = ip;
}


/***************************************************************
FUNCTION: tcpUtil::getConnectedClientSocket
IN: servIp Server's ip.
IN: servPort Server's port.

Creates and returns a connected client socket.

****************************************************************/
int tcpUtil::getConnectedClientSocket(const char* servIp, uint16_t servPort)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        return sock;

    struct sockaddr_in servAddr;
    bzero(&servAddr, sizeof(struct sockaddr_in));
    prepareAddressStruct(&servAddr, inet_addr(servIp), servPort);

    int rc = connect(sock, (const struct sockaddr *)&servAddr,
                   sizeof(servAddr));
    if (rc == -1) {
        perror("Connect Error: ");
        return -1;
    }

    std::cout << sock << " Client sock" << std::endl;
    return sock;
}

/***************************************************************
FUNCTION: tcpUtil::writeWithSizePrefix
IN: fd  File Descriptor to write to
IN: buff Data to  write

Writes buff to the file descriptor
****************************************************************/
void tcpUtil::writeWithSizePrefix(int fd, const std::string& buff)
{
    writeWithSizePrefix(fd, buff.c_str(), buff.size());
}

/***************************************************************
FUNCTION: tcpUtil::writeWithSizePrefix
IN: fd File descriptor
IN: buffer Buffer to be written
IN: sz Size of the buffer

Write buffer to fd
****************************************************************/
void tcpUtil::writeWithSizePrefix(int fd, const char *buffer, uint16_t sz)
{
    uint16_t nwBl = htons(sz);
    write(fd, &nwBl, sizeof(uint16_t));
    write(fd, buffer, sz);
}

/***************************************************************
FUNCTION: tcpUtil::readFromSocket
IN: fd  Socket
OUT: msg  Will contain the data read upon return

Reads from the socket
****************************************************************/

void tcpUtil::readFromSocket(int fd, std::string& msg)
{
    char buff[msgSizeLimit];
    uint16_t buffSz = msgSizeLimit;

    readFromSocket(fd, buff, buffSz);
    msg = std::string(buff, buffSz);
}

/***************************************************************
FUNCTION: tcpUtil::readFromSocket
IN: fd  Socket
OUT: msg  Will contain the data read upon return
OUT buffSz Will contain the size of the data upon return

Reads from the socket
****************************************************************/
void tcpUtil::readFromSocket(int fd, char *buffer, uint16_t& buffSz)
{
    uint16_t rdSz;
    int rc = read(fd, &rdSz, sizeof(rdSz));
    if (rc != sizeof(rdSz)) {
        buffSz = 0;
        return;
    }
    
    uint16_t hRdSz = ntohs(rdSz);
    if (hRdSz > buffSz) {
        uint32_t totRead = 0;
        while (totRead < hRdSz) {
            uint16_t rc = read(fd, buffer, buffSz);
            totRead += rc;
        }
        buffSz = 0;
        return;
    }

    buffSz = read(fd, buffer, buffSz);
}
