

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

#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

class Server
{
    private:

    int port;
    int sockfd;
    std::vector<Claint> claints;
    std::vector<pollfd> poll_fds;

    public:
    Server();
    void CreateSocket();

};

#endif
