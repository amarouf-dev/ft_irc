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

void Server::handle_nick(Client &client, const std::vector<std::string> &args)
{
    if (!client.IsAuthenticated())
    {
        sendToClient(client.GetFd(), "You must authenticate first with PASS\r\n");
        return;
    }

    if (args.size() < 2 || args[1].empty())
    {
        sendToClient(client.GetFd(), "No nickname given\r\n");
        return;
    }

    const std::string &nick_arg = args[1];
    if (isNickTaken(nick_arg))
    {
        sendToClient(client.GetFd(), "Nickname is already in use\r\n");
        return;
    }

    if (!isValidNickName(nick_arg))
    {
        sendToClient(client.GetFd(), "Not allowed chars for a nickname\r\n");
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
                sendToClient(clients[i].GetFd(), msg);
        }
    }

    sendToClient(client.GetFd(), "Nickname set to " + nick_arg + "\r\n");
}