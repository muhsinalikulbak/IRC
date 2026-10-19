#include <iostream>
#include <inttypes.h>
#include "Server.hpp"
#include <cerrno>

using std::cerr;
using std::cout;
using std::endl;

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        cerr << "Invalid Argument !" << endl;
        return 1;
    }
    if (strlen(argv[2]) == 0)
    {
        cerr << "Invalid Password !" << endl;
        return 1;
    }

    char* end;
    int port = strtoimax(argv[1], &end, 10);
    
    if (*end != '\0' || errno == ERANGE)
    {
        cerr << "Invalid Port Number !" << endl;
        return 1;
    }

    try
    {
        Server serv(argv[2]);
        serv.initServer(port);
        serv.ircFlow();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
