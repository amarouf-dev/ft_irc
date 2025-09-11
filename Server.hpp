

#ifndef SERVERRRR
#define SERVERRRR

#include "Claint.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#define RED "\e[1;31m"
#define GREEN "\e[1;32m"
#define WHITE "\e[0;37m"
#define YELLO "\e[1;33m"

class Server
{
    private:

    int port;
    int sockfd;
    std::vector<Claint> claints;
    std::vector<pollfd> poll_fds;
    void CreateSocket();
    void MainLoop();

    public:
    Server();
    void StartServer();
    void NewClaint();
    void NewData(int);
};

#endif
