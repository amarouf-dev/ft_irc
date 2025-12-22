#include "../headers/Server.hpp"

int Server::stop = 1;

Server::Server(int port, const std::string &password)
{
    this->port = htons(port);
    this->sockfd = -1;
    this->password = password;
    this->serverName = "ircserv";
    stop = 1;
}

Server::~Server() { cleanUp(); }

Channel* Server::getChannel(const std::string &channel_name)
{
    for (unsigned int i = 0; i < channels.size(); i ++)
    {
        if (channels[i]->getName() == channel_name)
        {
            return (channels[i]);
        }
    }
    return (NULL);
}

Client *Server::FindClaintByFd(int fd)
{
    for (unsigned long i = 0; i < clients.size(); i ++)
    {
        if (clients[i]->GetFd() == fd)
            return clients[i];
    }
    return (NULL);
}

Client *Server::FindClaintByName(std::string name)
{
     for (unsigned long i = 0; i < clients.size(); i ++)
    {
        if (clients[i]->GetNick() == name)
            return clients[i];
    }
    return (NULL);
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

void Server::removeClient(int fd)
{
    for (unsigned long i = 0; i < poll_fds.size(); i ++)
    {
        if (poll_fds[i].fd == fd)
        {
            poll_fds.erase(poll_fds.begin() + i);
            break;
        }
    }

    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->GetFd() == fd)
        {
            close(clients[i]->GetFd());
            delete clients[i];
            clients.erase(clients.begin() + i);
            break;
        }
    }
    close(fd);
}

void Server::signal_handler(int)
{
    std::cout << RED << "Server stops !" << WHITE << std::endl;
    stop = 0;
}

void Server::MainLoop()
{
    Client *c = NULL;

    signal(SIGINT, signal_handler);

    while (stop)
    {
        if (poll(&poll_fds[0], poll_fds.size(), -1) == -1)
            throw (std::runtime_error("Poll failed !"));
        for (unsigned long i = 0; i < poll_fds.size(); i ++)
        {
            if (poll_fds[i].revents & POLLIN)
            {
                if (poll_fds[i].fd == sockfd)
                    NewClient();
                else
                    NewData(poll_fds[i].fd);
            }
            if (poll_fds[i].revents & POLLOUT)
            {
                c = FindClaintByFd(poll_fds[i].fd);
                if (!c->GetBuffer().empty())
                {
                    ssize_t n = send(poll_fds[i].fd, c->GetBuffer().c_str(), c->GetBuffer().size(), 0);
                    if (n > 0)
                    {
                        c->GetBuffer().erase(0, n);
                        if (c->GetBuffer().empty())
                            poll_fds[i].events &= ~POLLOUT;
                    }
                }
            }
        }
    }
    cleanUp();
}

void Server::NewClient()
{
    sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);

    int new_fd = accept(sockfd, reinterpret_cast<sockaddr*>(&cliaddr), &len);
    if (new_fd == -1)
        throw std::runtime_error("Accept failed!");

    pollfd nc;
    nc.fd = new_fd;
    nc.events = POLLIN;
    nc.revents = 0;

    poll_fds.push_back(nc);
    
    Client* newC = new Client();
    newC->SetFd(new_fd);
    newC->SetIp(inet_ntoa(cliaddr.sin_addr));
    newC->SetServer(this);


    clients.push_back(newC);

    std::cout << GREEN << "Client (" << new_fd << ") Connected" << WHITE << std::endl;
}


void Server::NewData(int Cfd)
{
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));

    int bytes = recv(Cfd, buffer, 1023, 0);
    if (bytes <= 0)
    {
        if (bytes == 0)
        {
            std::cout << RED << "Client (" << Cfd << ") Disconnected !" << WHITE << std::endl;
            removeClient(Cfd);
            return;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
               return ;
            else if (errno == EINTR)
                return ;
            else
            {
                std::cout << RED << "Client (" << Cfd << ") Disconnected !" << WHITE << std::endl;
                removeClient(Cfd);
                return;
            }
        }
    }
    else
    {
        buffer[bytes] = '\0';
        for (size_t i = 0; i < clients.size(); i++)
        {
            if (clients[i]->GetFd() == Cfd)
            {
                std::string &buff = clients[i]->GetClientBuffer();
                buff += buffer;
                size_t pos;
                while ((pos = buff.find("\r\n")) != std::string::npos)
                {
                    std::string cmd = buff.substr(0, pos); 
                    buff.erase(0, pos + 2);                 
                    std::cout << "Received command: " << cmd << std::endl;
                    executeCmd(*clients[i], cmd);
                }
                break;
            }
        }
    }
}

void Server::cleanUp()
{
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if (clients[i])
        {
            close(clients[i]->GetFd());
            delete clients[i];
            clients[i] = NULL;
        }
    }
    clients.clear();

    for (size_t i = 0; i < channels.size(); ++i)
    {
        if (channels[i])
        {
            delete channels[i]; 
            channels[i] = NULL;
        }
    }
    channels.clear();

    close(sockfd);
    poll_fds.clear();
}
