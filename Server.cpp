

#include "Server.hpp"


Server::Server()
{
    this->port = htons(4040);
}

void Server::StartServer()
{
    CreateSocket();

    std::cout << GREEN << "Server (" << sockfd << ") 📡 Ready to accept connections..." << WHITE << std::endl;

    MainLoop();
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

void Server::MainLoop()
{
    while (true)
    {
        for (unsigned long i = 0; i < poll_fds.size(); i ++)
        {
            if (poll(&poll_fds[0], poll_fds.size(), -1) == -1)
                throw (std::runtime_error("Poll failed !"));
            if (poll_fds[i].revents == POLLIN)
            {
                if (poll_fds[i].fd == sockfd) // The sockfd is set to lesten (server socket) so it only tells if there is a new claint
                    NewClaint();
                else
                    NewData(poll_fds[i].fd);
            }
        }
    }
}

void Server::NewClaint()
{
    Claint NewC;
    pollfd  nc;

    sockaddr_in claint;
    socklen_t claint_len = sizeof(claint);
    nc.fd = accept(sockfd, reinterpret_cast<sockaddr *>(&claint), &claint_len);
    if (nc.fd == -1)
        throw (std::runtime_error("Accept failed !"));
    nc.events = POLLIN;
    nc.revents = 0;

    NewC.SetFd(nc.fd);
    NewC.SetIp(inet_ntoa(claint.sin_addr));

    poll_fds.push_back(nc);
    claints.push_back(NewC);

    std::cout << GREEN << "Client (" << nc.fd << ") Connected" << WHITE << std::endl;
}

void Server::NewData(int Cfd)
{
    char buffer[1024];

    std::memset(buffer, 0, sizeof(buffer));

    if (recv(Cfd, buffer, 1023, 0) <= 0)
    {
        std::cout << RED << "Claint (" << Cfd << ") Disconnected !" << std::endl;
    }
    else
    {
        std::cout << buffer;
    }
}


