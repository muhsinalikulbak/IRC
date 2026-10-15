#include "Server.hpp"

Server::Server()
{
    epollFd = -1;
    listenObject = NULL;
    events.reserve(100);
}

Server::~Server()
{
    
    if (listenObject)
    {
        epoll_ctl(epollFd, EPOLL_CTL_DEL, listenObject->getFd(), NULL);
        delete listenObject;
    }
    
    for (size_t i = 0; i < clientObjects.size(); i++)
    {
        if (clientObjects[i])
        {
            epoll_ctl(epollFd, EPOLL_CTL_DEL, clientObjects[i]->getFd(), NULL);
            delete clientObjects[i];
        }
    }
    
    if (epollFd != -1)
        close(epollFd);
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
    
    if (epollFd == -1)
        throw "epoll create error";

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, listenObject->getFd(), &event) == -1)
        throw "epoll ctrl error";
}

void Server::ircFlow()
{
    while (true)
    {
        int activeEvents = epoll_wait(epollFd, &events[0], events.size(), -1);

        for (size_t i = 0; i < activeEvents; i++)
        {
            ISocket* socket = (ISocket*) events[i].data.ptr;

            if (events[i].events & EPOLLIN && socket->getType() == LISTEN)
            {
                int fd = listenObject->acceptConnection();

                if (fd != -1)
                {
                    Client* client = new Client(fd);
                    epoll_event event;
                    event.events = EPOLLIN;
                    event.data.ptr = client;

                    epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);
                }
            }
            else if (events[i].events & EPOLLIN && socket->getType() == CLIENT)
            {
                Client* cli = (Client *) socket;
                cli->drawWater();
                if (cli->parseWater())
                {
                    events[i].events = EPOLLOUT;
                    epoll_ctl(epollFd, EPOLL_CTL_MOD, cli->getFd(), &events[i]);
                }
            }
            else if (events[i].events & EPOLLOUT)
            {
                if (socket->getType() == CLIENT)
                {
                    Client* cli = (Client *) socket;
                    if (cli->toPourWater())
                    {
                        events[i].events = EPOLLIN;
                        epoll_ctl(epollFd, EPOLL_CTL_MOD, cli->getFd(), &events[i]);
                    }
                }
                else
                {
                    perror ("Error listen socket");
                }
            }
        }

    }
    
}
