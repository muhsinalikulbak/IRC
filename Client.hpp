#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "ISocket.hpp"

using std::string;

class Client : public ISocket
{
private:
    int clientFd;
    string recvBuffer;
    string sendBuffer;

    string nick;
    string username;
    
    bool PASS;
    bool NICK;
    bool USER;
    bool isRegister;

public:
    Client(int fd);
    Client();
    ~Client();

    TYPE getType();
    int getFd();
    void setFd(int fd);

    void drawWater();
    bool toPourWater();
    bool parseWater();
    
};

#endif