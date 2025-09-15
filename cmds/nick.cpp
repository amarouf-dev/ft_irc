#include "../Server.hpp"


bool Server::isNickTaken(const std::string &nick)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].GetNick() == nick)
            return true;
    }
    return false;
}

//TODO: simple validation only letters numbers - _
bool Server::isValidNickName(const std::string &str)
{
    if (str.empty() || !isalpha(str[0]))
        return false;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (!isalpha(str[i]) && !isdigit(str[i]) && str[i] != '-' && str[i] != '_')
            return false;
    }
    return true;
}

void Server::handle_nick(Client &client, const std::string &nick_arg)
{
    if (!client.IsAuthenticated())
    {
        std::string rep = "You must authenticate first with PASS\r\n";
        send(client.GetFd(), rep.c_str(), rep.size(), 0);
        return;
    }

    if (nick_arg.empty())
    {
        std::string rep = "No nickname given\r\n";
        send(client.GetFd(), rep.c_str(), rep.size(), 0);
        return;
    }

    if (isNickTaken(nick_arg))
    {
        std::string rep = "Nickname is already in use\r\n";
        send(client.GetFd(), rep.c_str(), rep.size(), 0);
        return;
    }

    if (!isValidNickName(nick_arg))
    {
        std::string rep = "Not allowed chars for a nickname\r\n";
        send(client.GetFd(), rep.c_str(), rep.size(), 0);
        return;
    }

    std::string oldNick = client.GetNick();
    client.SetNick(nick_arg);

    if (!oldNick.empty() && oldNick != nick_arg)
    {
        std::string msg = ":" + oldNick + " NICK :" + nick_arg + "\r\n";
        for (size_t i = 0; i < clients.size(); i++)
        {
    
            
            if (clients[i].GetFd() != client.GetFd())
                send(clients[i].GetFd(), msg.c_str(), msg.size(), 0);
        }
    }

    std::string rep = "Nickname set to " + nick_arg + "\r\n";
    send(client.GetFd(), rep.c_str(), rep.size(), 0);
}