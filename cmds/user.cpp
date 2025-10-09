#include "../headers/Server.hpp"

bool Server::isValidUsername(const std::string &username)
{
    if (username.empty())
        return false;
    for (size_t i = 0; i < username.size(); i++)
    {
        char c = username[i];
        if (!isalnum(c) && c != '-' && c != '_' && c != '.')
            return false;
    }
    return true;
}

void Server::welcomeClient(Client &client)
{
    std::string nick = client.GetNick();
    std::string user = client.GetUsername();
    std::string host = client.GetIp();

    
    client.sendmsg(":" + serverName + " 001 " + nick + " :Welcome to the IRC network " +
        nick + "!" + user + "@" + host + "\r\n");

    client.sendmsg(":" + serverName + " 002 " + nick +
            " :Your host is " + serverName + ", running version 1.0\r\n");

    client.sendmsg(":" + serverName + " 003 " + nick +
            " :This server was created just now\r\n");

    client.sendmsg(":" + serverName + " 004 " + nick + " " + serverName +
            " 1.0 oi nt\r\n");
}

void Server::handle_user(Client &client, const std::vector<std::string> &args)
{
    if (!client.IsAuthenticated())
    {
        client.sendmsg("You must authenticate first with PASS\r\n");
        return;
    }

    if (args.size() < 5)
    {
        client.sendmsg("USER :Not enough parameters\r\n");
        return;
    }

    std::string username = args[1];
    if (!isValidUsername(username))
    {
        client.sendmsg("Not the right chars for username\r\n");
        return;
    }

    client.SetUsername(username);

    // hexchat special syntax sends :
    std::string realname = args[4];
    if (!realname.empty() && realname[0] == ':')
        realname = realname.substr(1);

    if (realname.empty())
    {
        client.sendmsg("USER :Realname is required\r\n");
        return;
    }

    client.SetRealname(realname);

    if (!client.GetNick().empty() && !client.GetUsername().empty() && !client.GetRealname().empty())
        welcomeClient(client);
}
