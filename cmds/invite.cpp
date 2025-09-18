
#include "../headers/Server.hpp"

void Server::handle_invite(Client &client, const std::vector<std::string> &args)
{
    std::string n_name = args[0];
    std::string ch_name = args[1];
    Channel *channel;
    

    if (!client.GetCurChannel())
    {
        std::cout << RED << "You must join a channel first!" << WHITE << std::endl;
    }

    channel = client.GetCurChannel();

    if (FindClaintByName(n_name))
    {
        Client *inv_c = FindClaintByName(n_name);
        std::string buf = "You have been invited to " + channel->getName() + " by " + client.GetNick() + "\n";
        send(inv_c->GetFd(), buf.c_str(), buf.size(), 0);
        channel->addClient(inv_c);
    }
    else
    {
        std::cout << RED << "Claint not found !" << WHITE << std::endl;
    }
}
