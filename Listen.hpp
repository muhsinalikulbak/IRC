#ifndef LISTEN_HPP
#define LISTEN_HPP

#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ISocket.hpp"
#include <fcntl.h>
using std::endl;
using std::cout;
using std::cerr;
using std::runtime_error;

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
