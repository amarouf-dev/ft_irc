

#include "../headers/Server.hpp"


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
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        throw (std::runtime_error("Socket Failed !"));
    
    sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = this->port;
    addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        throw (std::runtime_error("setsockopt Failed !"));

    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
        throw (std::runtime_error("fcntl Failed !"));

    if (bind(sockfd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
        throw (std::runtime_error("bind Failed !"));

    if (listen(sockfd, SOMAXCONN) == -1)
        throw (std::runtime_error("listen Failed !"));

    pollfd pfd;

    pfd.fd = sockfd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    poll_fds.push_back(pfd);
}

void Server::removeclaint(int fd)
{
    for (unsigned long i = 0; i < poll_fds.size(); i ++)
    {
        if (poll_fds[i].fd == fd)
        {
            poll_fds.erase(poll_fds.begin() + i);
        }
    }
    for (unsigned long i = 0; i < claints.size(); i ++)
    {
        if (claints[i].GetFd() == fd)
        {
            close (claints[i].GetFd());
            claints.erase(claints.begin() + i);
        }
    }
    close(fd);
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
                if (poll_fds[i].fd == sockfd)
                    NewClaint();
                else
                {
                    NewData(poll_fds[i].fd);
                }
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
    std::string msg;
    char buffer[BUFSIZE];

    std::memset(buffer, 0, sizeof(buffer));

    if (recv(Cfd, buffer, (BUFSIZE - 1), 0) <= 0)
    {
        std::cout << RED << "Claint (" << Cfd << ") Disconnected !" << std::endl;
        removeclaint(Cfd);
    }
    else
    {
        msg = buffer;
        std::cout << msg.size() << std::endl;
        std::cout << buffer;
    }
}


