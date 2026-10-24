#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "ISocket.hpp"
#include <vector>
#include <utility>

using std::pair;
using std::string;
using std::vector;

enum State {COMPLETE, INCOMPLETE, ERROR};

class Client : public ISocket
{
private:
    int clientFd;
    string recvBuffer;
    string sendBuffer;

    string nick;
    string username;
    
    bool PASS;
    bool NICK;
    bool USER;
    
public:
    Client(int fd);
    Client();
    ~Client();

    TYPE getType();
    int  getFd();
    void setFd(int fd);

    bool getPassState();
    bool getNickState();
    bool getUserState();


    const string& getNick();
    const string& getUser();
    void setNick(const string& nick);
    void setUser(const string& user);

    void setPassState(bool state);
    void setNickState(bool state);
    void setUserState(bool state);

    State toPourWater();
    bool drawWater();
    bool getLines(vector<string>& lines);
    bool isRegistered();
    
};

#endif