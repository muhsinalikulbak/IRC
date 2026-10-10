#ifndef ISOCKET_HPP
#define ISOCKET_HPP

enum TYPE { LISTEN, CLIENT };

class ISocket
{
public:
    virtual ~ISocket();
    virtual TYPE getType() = 0;
    virtual int getFd() = 0;
    virtual void setFd(int fd) = 0;

};

#endif