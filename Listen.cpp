#include "Listen.hpp"

Listen::Listen(int port)
{
    listenPort = port;
    listenFd = -1;
}

Listen::Listen()
{
    listenFd = -1;
}

Listen::~Listen()
{
    if (listenFd != -1)
        close(listenFd);
}
TYPE Listen::getType()
{
    return LISTEN;
}

int Listen::getFd()
{
    return listenFd;
}
void Listen::setFd(int fd)
{
    listenFd = fd;
}

void Listen::createSocket()
{
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd == -1)
        throw "socket error";
}

void Listen::bindSocket()
{
    sockaddr addr;
    addr.sa_family = AF_INET;

    if (bind(listenFd, &addr, sizeof(addr)) == -1)
        throw "bind error";
}
void Listen::listenSocket()
{
    // Ulimit ve backlog'u şöyle düşünebiliriz.
    // backlog = Listen'ın ikinci parametresi.
    // Ulimit = tek bir process'deki fd sınırıdır.

    // ****** [] <- <- <-

    // Yani ulimit içeride aynı anda en fazla bu kadar kişi bulunabilir demektir. (Socket sınırı)
    // Backlog ise, kapının dışında, aynı anda en fazla n kadar kişi bekleyebilir demektir.

    // İçeride boş yer varsa, fifo mantığıyla sıraya ilk giren accept olur.
    if (listen(listenFd, SOMAXCONN) == -1)
        throw ("listen error");
}

int  Listen::acceptConnection()
{
    sockaddr addr;
    addr.sa_family = AF_INET;
    socklen_t len = sizeof(addr);

    int clientFd = accept(listenFd, &addr, &len);

    if (clientFd == -1)
        throw "accept error";
}

