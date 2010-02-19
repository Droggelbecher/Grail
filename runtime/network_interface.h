
#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <vector>
#ifdef WIN32
#include <winsock.h>
#endif
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>

class NetworkInterface {
    uint32_t port;
    int32_t listenSocket, maxSocket;
    fd_set sockets;
    std::string chunk;

    void executeChunk(size_t n);

  public:
    NetworkInterface(uint32_t port);
    ~NetworkInterface();

    void bindLocal();
    void listen();
    void select();
}; 

#endif // NETWORK_INTERFACE_H

