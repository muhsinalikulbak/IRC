#include "Server.hpp"

Server::Server()
{

}

Server::~Server()
{

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
    // epoll_ctl(epollFd, EPOLL_CTL_ADD, listenObject->getFd(), )
}
