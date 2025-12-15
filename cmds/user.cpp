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

    
    client.sendmsg(Replies::RPL_WELCOME(serverName, nick, user, host));
    client.sendmsg(Replies::RPL_YOURHOST(serverName, nick));
    client.sendmsg(Replies::RPL_CREATED(serverName, nick));
    client.sendmsg(Replies::RPL_MYINFO(serverName, nick));
}

void Server::handle_user(Client &client, const std::vector<std::string> &args)
{
    if (!client.GetUsername().empty())
    {
        std::string reply = Replies::ERR_ALREADYREGISTERED(serverName, client.GetNick());
        client.sendmsg(reply);
        return;
    }

    if (args.size() < 5)
    {
        std::string reply = Replies::ERR_NEEDMOREPARAMS(serverName, client.GetNick(), "USER");
        client.sendmsg(reply);
        return;
    }

    std::string username = args[1];

    if (!isValidUsername(username))
    {
        client.sendmsg(":" + serverName + " NOTICE " + client.GetNick() + " :Invalid characters in username\r\n");
        return;
    }

    client.SetUsername(username);

    //!getBack2
    std::string realname = args[4];
    if (!realname.empty() && realname[0] == ':')
        realname = realname.substr(1);

    if (realname.empty())
    {
        std::string reply = Replies::ERR_NEEDMOREPARAMS(serverName, client.GetNick(), "USER");
        client.sendmsg(reply);
        return;
    }

    client.SetRealname(realname);
    std::cout << "Client realname set to: " << client.GetRealname() << std::endl;


    if (client.IsAuthenticated() &&  !client.GetNick().empty() && 
        !client.GetUsername().empty() && !client.GetRealname().empty())
        welcomeClient(client);
}
