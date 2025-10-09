#include "../headers/Server.hpp"

void Server::handle_join(Client &client, const std::vector<std::string> &args)
{
    if (!client.IsAuthenticated())
    {
        client.sendmsg("You must authenticate first with PASS");
        return;
    }

    if (args.size() < 2 || args[1].empty())
    {
        client.sendmsg("JOIN :Not enough parameters");
        return;
    }

    std::string channel_name = args[1];
    if (channel_name.empty() || (channel_name[0] != '#' && channel_name[0] != '&'))
    {
        client.sendmsg("JOIN :Invalid channel name");
        return;
    }

    Channel* chan = getOrCreateChannel(channel_name);
    if (!chan)
    {
        client.sendmsg("JOIN :Server error creating channel");
        return;
    }

    chan->addClient(&client);

    std::string prefix = ":" + client.GetNick() + "!" + client.GetUsername() 
                            + "@" + client.GetIp() + " JOIN " + channel_name + "\r\n";
    const std::set<Client *> &members = chan->getMembers();
    for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
    {
        Client* m = *it;
        if (m)
            m->sendmsg(prefix);
    }

    std::string names;
    for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
    {
        Client* m = *it;
        if (!names.empty())
            names += " ";
        if (chan->is_operator_in_channel(m))
            names += "@" + m->GetNick();   // operator p refix
        else
            names += m->GetNick();
    }

    std::string r353 = ":" + serverName + " 353 " + client.GetNick() +
        " = " + channel_name + " :" + names + "\r\n";
    std::string r366 = ":" + serverName + " 366 " + client.GetNick() +
        " " + channel_name + " :End of /NAMES list\r\n";

    client.sendmsg(r353);
    client.sendmsg(r366);

    std::cout << GREEN << client.GetNick() << " joined " << channel_name << WHITE << std::endl;
}


Channel* Server::getOrCreateChannel(const std::string &channel_name)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i]->getName() == channel_name)
            return channels[i];
    }
    Channel* new_chan = new Channel(channel_name);
    channels.push_back(new_chan);
    return new_chan;
}
