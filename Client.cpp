#include "Client.hpp"

Client::Client(int fd)
{
    clientFd = fd;
}
Client::Client()
{
    clientFd = -1;
}

Client::~Client()
{
    if (clientFd != -1)
        close(clientFd);
}

TYPE Client::getType()
{
    return CLIENT;
}
int Client::getFd()
{
    return clientFd;
}

void Client::setFd(int fd)
{
    clientFd = fd;
}

