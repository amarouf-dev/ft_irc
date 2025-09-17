

#include "../headers/Server.hpp"


Server::Server(int port, const std::string &password)
{
    this->port = htons(port);
    this->password = password;
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
        for (unsigned long i = 0; i < poll_fds.size(); i ++)
        {
            if (poll(&poll_fds[0], poll_fds.size(), -1) == -1)
                throw (std::runtime_error("Poll failed !"));
            if (poll_fds[i].revents == POLLIN)
            {
                if (poll_fds[i].fd == sockfd)
                    NewClient();
                else
                {
                    NewData(poll_fds[i].fd);
                }
            }
            if (poll_fds[i].revents & POLLOUT)
            {
                this->FindClaintByFd(poll_fds[i].fd);
                if (!c->GetBuffer().empty())
                {
                    ssize_t n = send(poll_fds[i].fd, c->GetBuffer().c_str(), c->GetBuffer().size(), 0);
                    if (n > 0)
                    {
                        c->GetBuffer().erase(0, n);
                        if (c->GetBuffer().empty())
                        {
                            poll_fds[i].events &= ~POLLOUT;
                        }
                    }
                }
            }
        }
    }
}

void Server::NewClient()
{
    Client NewC;
    pollfd  nc;

    sockaddr_in Client;
    socklen_t Client_len = sizeof(Client);
    nc.fd = accept(sockfd, reinterpret_cast<sockaddr *>(&Client), &Client_len);
    if (nc.fd == -1)
        throw (std::runtime_error("Accept failed !"));
    nc.events = POLLIN;
    nc.revents = 0;

    NewC.SetFd(nc.fd);
    NewC.SetIp(inet_ntoa(Client.sin_addr));
    NewC.SetPfd(&nc);
    NewC.SetCurChannel(NULL);
    poll_fds.push_back(nc);
    
    clients.push_back(NewC);

    std::cout << GREEN << "Client (" << nc.fd << ") Connected" << WHITE << std::endl;
}

// void Server::createchannel(std::string n, int fd)
// {
//     Channel ch(n);
//     Client *c;

//     for (unsigned long i = 0; i < clients.size(); i ++)
//     {
//         if (clients[i].GetFd() == fd)
//         {
//             c = &clients[i];
//         }
//     }

//     for (unsigned long i = 0; i < channels.size(); i ++)
//     {
//         if (channels[i]->getName() == n)
//         {
//             channels[i]->join_channel(c);
//             return ;
//         }
//     }
//     ch.Initchannel(n, c);
//     this->channels.push_back(&ch);
// }


void Server::NewData(int Cfd)
{
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));

    int bytes = recv(Cfd, buffer, 1023, 0);
    if (bytes <= 0)
    {
        std::cout << RED << "Client (" << Cfd << ") Disconnected !" << WHITE << std::endl;
        // TODO: maybe remove client from lists
        return;
    }

    std::string input(buffer);
    std::cout << "Received: " << input << std::endl;  // debug

    // tokenize inpt
    std::istringstream iss(input);
    std::string command;
    iss >> command;

    for (size_t i = 0; i < command.size(); i++)
        command[i] = toupper(command[i]);

    Client* client = NULL;
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].GetFd() == Cfd)
        {
            client = &clients[i];
            break;
        }
    }
    // if (!client) return;

    if (command == "PASS")
    {
        std::string pass_arg;
        iss >> pass_arg;
        handle_pass(*client, pass_arg);
    }
    else if (command == "NICK")
    {
        std::string nick_arg;
        iss >> nick_arg;
        handle_nick(*client, nick_arg);
    }
    else if (command == "USER")
    {
        std::vector<std::string> args;
        std::string arg;
        while (iss >> arg) args.push_back(arg);
        handle_user(*client, args);
    }
    else if (command == "JOIN")
    {
        std::string chan;
        iss >> chan;
        handle_join(*client, chan);
    }
    // else if (command == "TOPIC")
    // {
    //     std::string tpc;
    //     iss >> tpc;
    //     handle_topic(*client, tpc);
    // }
    else if (command == "KICK")
    {
        std::vector<std::string> Kargs;
        std::string Karg;
        while (iss >> Karg) Kargs.push_back(Karg);
        handle_kick(*client, Kargs);
    }
    else
    {
        std::string rep = "Unknown command\r\n";
        send(Cfd, rep.c_str(), rep.size(), 0);
    }
}

