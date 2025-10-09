

#include "../headers/Server.hpp"


Server::Server(int port, const std::string &password)
{
    this->port = htons(port);
    this->password = password;
    this->serverName = "ircserv";
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
        if (clients[i].GetFd() == fd)
        {
            return (&clients[i]);
        }
    }
    return (NULL);
}

Client *Server::FindClaintByName(std::string name)
{
     for (unsigned long i = 0; i < clients.size(); i ++)
    {
        if (clients[i].GetNick() == name)
        {
            return (&clients[i]);
        }
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
        }
    }
    for (unsigned long i = 0; i < clients.size(); i ++)
    {
        if (clients[i].GetFd() == fd)
        {
            close (clients[i].GetFd());
            clients.erase(clients.begin() + i);
        }
    }
    close(fd);
}

void Server::MainLoop()
{
    Client *c = NULL;

    while (true)
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
                    //the actual sending happens heere
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

    Client newC;
    newC.SetFd(new_fd);
    newC.SetIp(inet_ntoa(cliaddr.sin_addr));
    // newC.SetCurChannel(NULL);
    newC.SetPfd(&poll_fds.back());

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
        std::cout << RED << "Client (" << Cfd << ") Disconnected !" << WHITE << std::endl;
        removeClient(Cfd);
        return;
    }
    else
    {
        buffer[bytes] = '\0';
        for (size_t i = 0; i < clients.size(); i++)
        {
            if (clients[i].GetFd() == Cfd)
            {
                std::string &buff = clients[i].GetClientBuffer();
                buff += buffer;
                size_t pos;
                while ((pos = buff.find("\r\n")) != std::string::npos)
                {
                    std::string cmd = buff.substr(0, pos); //eextract one command 
                    buff.erase(0, pos + 2);                 
                    std::cout << "Received command: " << cmd << std::endl;
                    executeCmd(clients[i], cmd);
                }
                break;
            }
        }
    }

}


void Server::cleanUp()
{
    // close client sockets
    for (size_t i = 0; i < clients.size(); ++i)
    {
        close(clients[i].GetFd());
    }
    clients.clear();

    for (size_t i = 0; i < channels.size(); ++i)
    {
        delete channels[i];
    }
    channels.clear();

    // close server socket
    if (sockfd != -1)
        close(sockfd);

    poll_fds.clear();
}





// void Server::NewData(int Cfd)
// {
//     char buffer[1024];
//     // std::memset(buffer, 0, sizeof(buffer));

//     int bytes = recv(Cfd, buffer, 1023, 0);
//     if (bytes <= 0)
//     {
//         std::cout << RED << "Client (" << Cfd << ") Disconnected !" << WHITE << std::endl;
//         removeClient(Cfd);
//         return;
//     }

//     // std::string input(buffer);
//     // std::cout << "Received: " << input << std::endl;  // debug

//     // tokenize inpt
//     // std::istringstream iss(input);
//     // std::string command;
//     // iss >> command;

//     // for (size_t i = 0; i < command.size(); i++)
//     //     command[i] = toupper(command[i]);

//     // Client* client = FindClaintByFd(Cfd);

//     buffer[bytes] = '\0';
//     Client* c = FindClaintByFd(Cfd);
//     if (!c) 
//         return;

//     c->GetClientBuffer() += buf;

//     size_t pos;
//     while ((pos = c->GetClientBuffer().find("\r\n")) != std::string::npos)
//     {
//         std::string cmd = c->GetClientBuffer().substr(0, pos);
//         c->GetClientBuffer().erase(0, pos + 2);
//         executeCmd(*c, cmd);
//     }

//     // if (command == "PASS")
//     // {
//     //     std::string pass_arg;
//     //     iss >> pass_arg;
//     //     handle_pass(*client, pass_arg);
//     // }
//     // else if (command == "NICK")
//     // {
//     //     std::string nick_arg;
//     //     iss >> nick_arg;
//     //     handle_nick(*client, nick_arg);
//     // }
//     // else if (command == "USER")
//     // {
//     //     std::vector<std::string> args;
//     //     std::string arg;
//     //     while (iss >> arg) args.push_back(arg);
//     //     handle_user(*client, args);
//     // }
//     // else if (command == "JOIN")
//     // {
//     //     std::string chan;
//     //     iss >> chan;
//     //     handle_join(*client, chan);
//     // }
//     // else if (command == "TOPIC")
//     // {
//     //     std::vector<std::string> targs;
//     //     std::string targ;
//     //     while (iss >> targ) targs.push_back(targ);
//     //     handle_topic(*client, targs);
//     // }
//     // else if (command == "INVITE")
//     // {
//     //     std::vector<std::string> iargs;
//     //     std::string iarg;
//     //     while (iss >> iarg) iargs.push_back(iarg);
//     //     handle_invite(*client, iargs);
//     // }
//     // else if (command == "KICK")
//     // {
//     //     std::vector<std::string> Kargs;
//     //     std::string Karg;
//     //     while (iss >> Karg) Kargs.push_back(Karg);
//     //     handle_kick(*client, Kargs);
//     // }
//     // else
//     // {
//     //     std::string rep = "Unknown command\r\n";
//     //     client->sendmsg(rep);
//     // }
// }
