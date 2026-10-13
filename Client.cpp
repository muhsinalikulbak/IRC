#include "Client.hpp"

Client::Client(int fd)
{
    clientFd = fd;
    recvBuffer = "";
    sendBuffer = "";
    
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


void Client::drawWater()
{
    char buffer[4096];

    int bytes = recv(clientFd, buffer, 4096, 0);

    if (bytes > 0)
    {
        recvBuffer.append(buffer);
    }
    else if (bytes == -1)
    {
        
    }
}
void Client::parseWater()
{
    if (recvBuffer.empty() || recvBuffer.size() < 1)
        return;
    
    size_t pos = recvBuffer.find("\r\n");

    if (pos == std::string::npos)
        return;
    
    sendBuffer.append(recvBuffer.substr(0, pos));
    recvBuffer.erase(0, pos);
}
