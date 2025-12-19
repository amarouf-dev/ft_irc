
#include "../headers/Server.hpp"

void Server::handle_kick(Client &client, const std::vector<std::string> &args)
{
    if (args.size() < 3)
    {
        std::string reply = Replies::ERR_NEEDMOREPARAMS(serverName, client.GetNick(), "KICK");
        client.sendmsg(reply);
        return;
    }

    std::string ch_name = args[1];
    std::string users_list = args[2];
    std::vector<std::string> users_to_kick;
    std::string reason;

    size_t pos = 0;
    size_t comma_pos;

    while ((comma_pos = users_list.find(',', pos)) != std::string::npos)
    {
        std::string ch = users_list.substr(pos, comma_pos - pos);
        ch = trim(ch);
        if (!ch.empty())
            users_to_kick.push_back(ch);
        pos = comma_pos + 1;
    }

    std::string last_ch = users_list.substr(pos);
    last_ch = trim(last_ch);
    if (!last_ch.empty())
        users_to_kick.push_back(last_ch);

    if (args.size() > 3)
    {
        std::string new_topic = args[3];
        if (!new_topic.empty() && new_topic[0] == ':')
            new_topic.erase(0, 1);
        reason = new_topic;
        for (size_t i = 4; i < args.size(); i++)
            reason += " " + args[i];
    }
    else
        reason = client.GetNick();

    Channel *chnl = getChannel(ch_name);
    if (!chnl)
    {
        std::string reply = Replies::ERR_NOSUCHCHANNEL(serverName, client.GetNick(), ch_name);
        client.sendmsg(reply);        
        return;
    }

    if (!chnl->is_member(&client))
    {
        std::string reply = Replies::ERR_NOTONCHANNEL(serverName, client.GetNick(), ch_name);
        client.sendmsg(reply);     
        return;
    }

    if (!chnl->isoperator(client.GetNick()))
    {
        std::string reply = Replies::ERR_CHANOPRIVSNEEDED(serverName, client.GetNick(), ch_name);
        client.sendmsg(reply);        
        return;
    }

    for (size_t i = 0; i < users_to_kick.size(); i ++)
    {
        Client *target = chnl->GetMemberByName(users_to_kick[i]);
    
        if (!target)
        {
            std::string reply = Replies::ERR_USERNOTINCHANNEL(serverName, client.GetNick(), users_to_kick[i], ch_name);
            client.sendmsg(reply);
            continue;
        }
    
        std::string kickMsg = ":" + client.GetNick() + "!" + client.GetUsername() +
                              "@" + client.GetIp() + " KICK " + ch_name + " " +
                              users_to_kick[i] + " :" + reason + "\r\n";
    
        chnl->broadcast(kickMsg);
        chnl->removeClaint(target);
    }
}
