#include "../headers/Server.hpp"


void Server::handle_part(Client &client, const std::vector<std::string> &args)
{
    if (!client.IsAuthenticated())
    {
        std::string reply = Replies::ERR_NOTREGISTERED(serverName, client.GetNick());
        client.sendmsg(reply);
        return;
    }

    if (args.size() < 2 || args[1].empty())
    {
        std::string reply = Replies::ERR_NEEDMOREPARAMS(serverName, client.GetNick(), "PART");
        client.sendmsg(reply);
        return;
    }

    std::string channel_name = args[1];
    if (channel_name.empty() || (channel_name[0] != '#' && channel_name[0] != '&'))
    {
        std::string reply = Replies::ERR_NOSUCHCHANNEL(serverName, client.GetNick(), channel_name);
        client.sendmsg(reply);
        return;
    }

    Channel* chan = getChannel(channel_name);
    if (!chan)
    {
        std::string reply = Replies::ERR_NOSUCHCHANNEL(serverName, client.GetNick(), channel_name);
        client.sendmsg(reply);
        return;
    }

    if (!chan->is_member(&client))
    {
        std::string reply = Replies::ERR_NOTONCHANNEL(serverName, client.GetNick(), channel_name);
        client.sendmsg(reply);
        return;
    }

    std::string part_message;
    if (args.size() >= 3)
    {
        part_message = args[2];
        // remove : if present
        if (!part_message.empty() && part_message[0] == ':')
            part_message = part_message.substr(1);
    }

    std::string part_msg = ":" + client.GetNick() + "!" + client.GetUsername() 
                          + "@" + client.GetIp() + " PART " + channel_name;
    
    if (!part_message.empty())
        part_msg += " :" + part_message;
    
    part_msg += "\r\n";

    const std::set<Client*>& members = chan->getMembers();
    for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
    {
        (*it)->sendmsg(part_msg);
    }

    chan->removeClaint(&client);

    std::cout << RED << client.GetNick() << " left channel " << channel_name << WHITE << std::endl;

    // think other way: clean up empty channels
    if (chan->getMembers().empty())
    {
        for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            if (*it == chan)
            {
                delete chan;
                channels.erase(it);
                break;
            }
        }
    }
}