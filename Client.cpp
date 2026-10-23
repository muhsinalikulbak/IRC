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
// Çektiğin suyu kontrol et kaç adet line var
bool Client::getLines(vector<string>& lines)
{    
    if (recvBuffer.empty() || recvBuffer.size() < 2)
        return false;

    size_t pos = recvBuffer.find("\r\n");

    while (pos != std::string::npos)
    {
        lines.push_back(recvBuffer.substr(0, pos));
        recvBuffer.erase(0, pos + 2);
    
        pos = recvBuffer.find("\r\n");
    }
    
    return lines.size() > 0;
}


bool Client::isRegistered()
{
    return PASS && NICK && USER;
}



bool Client::getPassState()
{
    return PASS;
}
bool Client::getNickState()
{
    return NICK;
}
bool Client::getUserState()
{
    return USER;
}

void Client::setPassState(bool state)
{
    PASS = state;
}
void Client::setNickState(bool state)
{
    NICK = state;
}
void Client::setUserState(bool state)
{
    USER = state;
}



const string& Client::getNick()
{
    return nick;
}

const string& Client::getUser()
{
    return username;
}

void Client::setNick(const string& nick)
{
    this->nick = nick;
}
void Client::setUser(const string& user)
{
    this->username = user;
}
