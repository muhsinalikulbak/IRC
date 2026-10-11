#include "Server.hpp"

Server::Server()
{

}

Server::~Server()
{
    if (epollFd != -1)
        close(epollFd);
    
    // epoll_ctl(epollFd, EPOLL_CTL_DEL, listenObject->getFd(), NULL);

    delete listenObject;

    for (size_t i = 0; i < clientObjects.size(); i++)
    {
        if (clientObjects[i])
            delete clientObjects[i];
    }
    
}

void Server::initServer(int port)
{
    listenObject = new Listen(port);

    listenObject->createSocket();
    listenObject->bindSocket();
    listenObject->listenSocket();

    epoll_event event;
    event.events = EPOLLIN;
    event.data.ptr = listenObject;

    epollFd = epoll_create(0);

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, listenObject->getFd(), &event) == -1)
        throw "epoll ctrl error";
}
void Server::ircFlow()
{
    
    while (true)
    {
        

    }
    
}
