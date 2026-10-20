#include "Client.hpp"

Client::Client(int fd)
{
    clientFd = fd;
    recvBuffer = "";
    sendBuffer = "";
    nick = "";
    username = "";
    PASS = false;
    NICK = false;
    USER = false;
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

// Suyu Kernel Recv Buffer'dan çek
bool Client::drawWater()
{
    char buffer[4096];

    int bytes = recv(clientFd, buffer, 4096, 0);

    if (bytes > 0)
    {
        recvBuffer.append(buffer, bytes);
        return true;
    }
    else if (bytes == -1)
    {
        perror("recv error");
        return false;
    }
    else if (bytes == 0)
        return false;   // Karşı tarafın kapatma sinyali.

    return false;
}

// Suyu Kernel Send Buffer'a dök
State Client::toPourWater()
{
    int bytes = send(clientFd, sendBuffer.c_str(), sendBuffer.size(), 0);

    if (bytes > 0)
    {
        sendBuffer.erase(0, bytes);
        return sendBuffer.size() == 0 ? COMPLETE : INCOMPLETE;
    }
    else if (bytes == 0)
    {
        return COMPLETE;
    }
    else if (bytes == -1)
    {
        perror("send error");
        return ERROR;
    }
    return ERROR;
}
// Çektiğin suyu kontrol et
bool Client::parseWater()
{    
    if (recvBuffer.empty() || recvBuffer.size() < 2)
        return false;
    
    size_t pos = recvBuffer.find("\r\n");

    if (pos == std::string::npos)
        return false;
    
    sendBuffer.append(recvBuffer.substr(0, pos + 2)); 
    recvBuffer.erase(0, pos + 2);
    return true;
}
