
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
    std::string targetNick = args[2];
    std::string reason;

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
    Client *target = chnl->GetMemberByName(targetNick);

    if (!target)
    {
        std::string reply = Replies::ERR_USERNOTINCHANNEL(serverName, client.GetNick(), ch_name, ch_name);
        client.sendmsg(reply);
        return ;
    }

    std::string kickMsg = ":" + client.GetNick() + "!" + client.GetUsername() +
                          "@" + client.GetIp() + " KICK " + ch_name + " " +
                          targetNick + " :" + reason + "\r\n";

    chnl->broadcast(kickMsg);
    chnl->removeClaint(target);
}
