#include "../headers/Server.hpp"

void Server::joinSingleChannel(Client &client, const std::string &channel_name, const std::string &key)
{
    Channel* chan = getOrCreateChannel(channel_name);
    if (!chan)
    {
        client.sendmsg(":" + serverName + " NOTICE " + client.GetNick() + " :Server error creating channel\r\n");
        return;
    }

    if (chan->is_member(&client))
        return;

        
    if (chan->GetInviteonly())
    {
        if (!chan->is_invited(&client))
        {
            std::string reply = Replies::ERR_INVITEONLYCHAN(serverName, client.GetNick(), channel_name);
            client.sendmsg(reply);
            return;
        }
    }
        
    if (chan->hasKey())
    {
        if (key.empty() || key != chan->getKey())
        {
            std::string reply = Replies::ERR_BADCHANNELKEY(serverName, client.GetNick(), channel_name);
            client.sendmsg(reply);
            return;
        }
    }

    if (chan->hasUserLimit() && chan->getMembers().size() >= chan->getUserLimit())
    {
        std::string reply = Replies::ERR_CHANNELISFULL(serverName, client.GetNick(), channel_name);
        client.sendmsg(reply);
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

    if (!chan->GetTopic().empty())
    {
        std::string topic_msg = ":" + serverName + " 332 " + client.GetNick() + " " +
                               channel_name + " :" + chan->GetTopic() + "\r\n";
        client.sendmsg(topic_msg);
    }

    std::string names;
    for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
    {
        Client* m = *it;
        if (!names.empty())
            names += " ";
        if (chan->is_operator_in_channel(m))
            names += "@" + m->GetNick();
        else
            names += m->GetNick();
    }

    client.sendmsg(Replies::RPL_NAMREPLY(serverName, client.GetNick(), channel_name, names));
    client.sendmsg(Replies::RPL_ENDOFNAMES(serverName, client.GetNick(), channel_name));

    std::cout << GREEN << client.GetNick() << " joined " << channel_name << WHITE << std::endl;
}


void Server::handle_join(Client &client, const std::vector<std::string> &args)
{
    if (!client.IsAuthenticated())
    {
        std::string reply = Replies::ERR_NOTREGISTERED(serverName, client.GetNick());
        client.sendmsg(reply);
        return;
    }

    if (args.size() < 2 || args[1].empty())
    {
        std::string reply = Replies::ERR_NEEDMOREPARAMS(serverName, client.GetNick(), "JOIN");
        client.sendmsg(reply);
        return;
    }
    
    std::vector<std::string> channels_to_join;
    std::string channel_list = args[1];
    size_t pos = 0;
    size_t comma_pos;
    
    while ((comma_pos = channel_list.find(',', pos)) != std::string::npos)
    {
        std::string ch = channel_list.substr(pos, comma_pos - pos);
        ch = trim(ch);
        if (!ch.empty())
            channels_to_join.push_back(ch);
        pos = comma_pos + 1;
    }

    std::string last_ch = channel_list.substr(pos);
    last_ch = trim(last_ch);
    if (!last_ch.empty())
        channels_to_join.push_back(last_ch);

    std::vector<std::string> keys;
    if (args.size() >= 3 && !args[2].empty())
    {
        std::string key_list = args[2];
        pos = 0;
        while ((comma_pos = key_list.find(',', pos)) != std::string::npos)
        {
            std::string k = key_list.substr(pos, comma_pos - pos);
            k = trim(k);
            keys.push_back(k);
            pos = comma_pos + 1;
        }
        std::string last_key = key_list.substr(pos);
        last_key = trim(last_key);
        keys.push_back(last_key);
    }

    for (size_t i = 0; i < channels_to_join.size(); i++)
    {
        std::string channel_name = channels_to_join[i];
        
        if (channel_name.empty() || (channel_name[0] != '#' && channel_name[0] != '&'))
        {
            std::string reply = Replies::ERR_BADCHANMASK(serverName, client.GetNick(), channel_name);
            client.sendmsg(reply);
            continue;
        }

        std::string key;

        if (i < keys.size())
            key = keys[i];
        else
            key = "";

        joinSingleChannel(client, channel_name, key);
    }
    channels_to_join.clear();
    keys.clear();
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
