#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include "Client.hpp"
#include "Listen.hpp"
#include <sys/epoll.h>

using std::vector;
 
class Server
{
private:
    vector<epoll_event> events;
    vector<Client*> clientObjects;
    Listen* listenObject;
    int epollFd;

public:
    Server();
    ~Server();
    void initServer(int port);

};

#endif