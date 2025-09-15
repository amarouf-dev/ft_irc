#include "../Server.hpp"

// only simple chars allowed
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

void Server::handle_user(Client &client, const std::vector<std::string> &args)
{
    if (!client.IsAuthenticated())
    {
        std::string rep = "You must authenticate first with PASS\r\n";
        send(client.GetFd(), rep.c_str(), rep.size(), 0);
        return;
    }

    if (args.size() < 5)
    {
        std::string rep = "USER :Not enough parameters\r\n";
        send(client.GetFd(), rep.c_str(), rep.size(), 0);
        return;
    }

    std::string username = args[1];
    if (!isValidUsername(username))
    {
        std::string rep = "not the right chars for username\r\n";
        send(client.GetFd(), rep.c_str(), rep.size(), 0);
        return;
    }

    client.SetUsername(username);

    // hexchat special syntax sends :
    std::string realname = args[4];
    if (!realname.empty() && realname[0] == ':')
        realname = realname.substr(1);

    if (realname.empty())
    {
        std::string rep = "USER :Realname is required\r\n";
        send(client.GetFd(), rep.c_str(), rep.size(), 0);
        return;
    }

    client.SetRealname(realname);

    if (!client.GetNick().empty() && !client.GetUsername().empty() && !client.GetRealname().empty())
        welcomeClient(client);
}

void Server::welcomeClient(Client &client)
{
    std::string rep = "Welcome " + client.GetNick() + "!" + client.GetUsername() + "\r\n";
    send(client.GetFd(), rep.c_str(), rep.size(), 0);
}