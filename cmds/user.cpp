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

    
    sendToClient(client.GetFd(),    //RPL_WELCOME
        ":" + serverName + " 001 " + nick + " :Welcome to the IRC network " +
        nick + "!" + user + "@" + host);

    sendToClient(client.GetFd(),    //RPL_YOURHOST
        ":" + serverName + " 002 " + nick +
        " :Your host is " + serverName + ", running version 1.0");

    sendToClient(client.GetFd(),     //RPL_CREATED
        ":" + serverName + " 003 " + nick +
        " :This server was created just now");

    sendToClient(client.GetFd(),    //rPL_MYINFO
        ":" + serverName + " 004 " + nick + " " + serverName +
        " 1.0 oi nt"); 
}

void Server::handle_user(Client &client, const std::vector<std::string> &args)
{
    if (!client.IsAuthenticated())
    {
        sendToClient(client.GetFd(), "You must authenticate first with PASS\r\n");
        return;
    }

    if (args.size() < 5)
    {
        sendToClient(client.GetFd(), "USER :Not enough parameters\r\n");
        return;
    }

    std::string username = args[1];
    if (!isValidUsername(username))
    {
        sendToClient(client.GetFd(), "Not the right chars for username\r\n");
        return;
    }

    client.SetUsername(username);

    // hexchat special syntax sends :
    std::string realname = args[4];
    if (!realname.empty() && realname[0] == ':')
        realname = realname.substr(1);

    if (realname.empty())
    {
        sendToClient(client.GetFd(), "USER :Realname is required\r\n");
        return;
    }

    client.SetRealname(realname);

    if (!client.GetNick().empty() && !client.GetUsername().empty() && !client.GetRealname().empty())
        welcomeClient(client);
}
