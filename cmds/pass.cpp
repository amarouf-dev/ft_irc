#include "../headers/Server.hpp"

void Server::handle_pass(Client &client, const std::vector<std::string> &args)
{

    if (!client.GetUsername().empty() && !client.GetNick().empty() && client.IsAuthenticated())
    {
        std::string reply = Replies::ERR_ALREADYREGISTERED(serverName, client.GetNick());
        client.sendmsg(reply);
        return;
    }

    if (args.size() < 2 || args[1].empty())
    {
        std::string reply = Replies::ERR_NEEDMOREPARAMS(serverName, client.GetNick(), "PASS");
        client.sendmsg(reply);
        return;
    }

    const std::string &pass_arg = args[1];

    if (pass_arg == password)
    {
        client.Authenticate();
        std::cout << GREEN << "Client (" << client.GetFd() << ") authenticated successfully" << WHITE << std::endl;
    
        if (!client.GetNick().empty() && !client.GetUsername().empty() && 
            !client.GetRealname().empty())
            welcomeClient(client);
    }
    else
    {
        std::string reply = Replies::ERR_PASSWDMISMATCH(serverName, client.GetNick());
        client.sendmsg(reply);
    }
}