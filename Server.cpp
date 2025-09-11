

#include "Server.hpp"


Server::Server()
{
    this->port = htons(4040);

    CreateSocket();

    std::cout << GRE << "Server " << sockfd << " ready to accept connections ..." << WHI << std::endl;
}

void Server::CreateSocket()
{
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0); // Socket clreation
    if (sockfd == -1)
        throw (std::runtime_error("Socket Failed !"));
    
    sockaddr_in addr; // set the socket's options to bind it

    addr.sin_family = AF_INET; // IPV4
    addr.sin_port = this->port;
    addr.sin_addr.s_addr = INADDR_ANY; // bind it to all the interfaces on the device

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) // set socket options
        throw (std::runtime_error("setsockopt Failed !"));

    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) // gives the socket a non_blocking option (it doesn't wait for data to be recvd)
        throw (std::runtime_error("fcntl Failed !"));

    if (bind(sockfd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1) // gives the socket a port and an address
        throw (std::runtime_error("bind Failed !"));

    if (listen(sockfd, SOMAXCONN) == -1)
        throw (std::runtime_error("listen Failed !"));

    pollfd pfd;

    pfd.fd = sockfd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    poll_fds.push_back(pfd);
}

