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
        perror("recv error");
    }
}


void Client::toPourWater()
{
    int bytes = send(clientFd, sendBuffer.c_str(), sendBuffer.size(), 0);

    if (bytes > 0)
    {
        sendBuffer.erase(0, bytes);
    }
    else if (bytes == -1)
    {
        perror("send error");
    }
}

bool Client::parseWater()
{
    if (recvBuffer.empty() || recvBuffer.size() < 2)
        return false;
    
    size_t pos = recvBuffer.find("\r\n");

    if (pos == std::string::npos)
        return false;
    
    sendBuffer.append(recvBuffer.substr(0, pos));
    recvBuffer.erase(0, pos + 2);
    return true;
}
