#ifndef LISTEN_HPP
#define LISTEN_HPP

#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include "ISocket.hpp"

using std::endl;
using std::cout;
using std::cerr;

class Listen : public ISocket
{
private:
    int listenFd;
    int listenPort;

public:
    Listen(int port);
    Listen();
    ~Listen();

    TYPE getType();
    int  getFd();
    void setFd(int fd);

    void createSocket();
    void bindSocket();
    void listenSocket();
    int  acceptConnection();
};

#endif
