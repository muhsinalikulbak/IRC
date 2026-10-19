#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include "Client.hpp"
#include "Listen.hpp"
#include <sys/epoll.h>
#include <cerrno>
#include <map>

using std::runtime_error;
using std::vector;
using std::map;

class Server
{
private:
    vector<epoll_event> events;
    vector<Client*> clientObjects;
    Listen* listenObject;
    int epollFd;

    map<string, Client*> nickMap;
    string password;

public:
    Server(string password);
    ~Server();
    void initServer(int port);
    void ircFlow();

};

#endif