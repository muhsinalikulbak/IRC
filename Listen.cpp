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
        throw runtime_error(strerror(errno));
    
    int opt = 1;
    fcntl(listenFd, F_SETFL, O_NONBLOCK);
    setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Listen::bindSocket()
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(listenPort);


    if (bind(listenFd, (sockaddr*)&addr, sizeof(addr)) == -1)
        throw runtime_error (strerror(errno));
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
        throw runtime_error(strerror(errno));
}

int  Listen::acceptConnection()
{
    // Client IP'sini loglamak istersem sockaddr_in ile parametreyi doldur

    int clientFd = accept(listenFd, NULL, NULL);
    
    if (clientFd == -1)
        cerr << "Accept error" << endl;

    fcntl(clientFd, F_SETFL, O_NONBLOCK);
    return clientFd;
}

