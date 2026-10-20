#include "Server.hpp"

Server::Server(string password)
{
    this->password = password;
    epollFd = -1;
    listenObject = NULL;
    events.resize(100);
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
    
    epollFd = epoll_create1(0);
    if (epollFd == -1)
    {
        throw runtime_error(strerror(errno));
    }
    
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, listenObject->getFd(), &event) == -1)
        throw runtime_error(strerror(errno));
}

void Server::ircFlow()
{
    while (true)
    {
        int activeEvents = epoll_wait(epollFd, &events[0], events.size(), -1);
        
        if (activeEvents == -1)
        {
            throw runtime_error(strerror(errno));
        }

        for (int i = 0; i < activeEvents; i++)
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
                    clientObjects.push_back(client);
                }
            }
            else if (events[i].events & EPOLLIN && socket->getType() == CLIENT)
            {
                Client* cli = (Client *) socket;
                if (cli->drawWater())
                {
                    if (cli->parseWater())
                    {
                        events[i].events = EPOLLOUT;
                        epoll_ctl(epollFd, EPOLL_CTL_MOD, cli->getFd(), &events[i]);
                    }
                }
                else
                    unregisterClient(cli);

            }
            else if (events[i].events & EPOLLOUT)
            {
                if (socket->getType() == CLIENT)
                {
                    Client* cli = (Client *) socket;
                    State st = cli->toPourWater();

                    if (st == COMPLETE)
                    {
                        events[i].events = EPOLLIN;
                        epoll_ctl(epollFd, EPOLL_CTL_MOD, cli->getFd(), &events[i]);
                    }
                    else if (st == ERROR) 
                        unregisterClient(cli);
                }
                else
                {
                    perror ("Error listen socket");
                }
            }
        }

    }
}

void Server::unregisterClient(Client* cli)
{
    // Channel olunca, kayıtlı olduğu kanallardan da silinecek

    epoll_ctl(epollFd, EPOLL_CTL_DEL, cli->getFd(), NULL);
    vector<Client*>::iterator it = find(clientObjects.begin(), clientObjects.end(), cli);

    if (it != clientObjects.end())
    {
        *it = clientObjects.back();
        clientObjects.pop_back();
    }
    delete cli;
}
