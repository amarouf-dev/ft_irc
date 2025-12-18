
#include "../headers/Server.hpp"

void Server::handle_kick(Client &client, const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        client.sendmsg(":ircserv 461 " + client.GetNick() + " KICK :Not enough parameters\r\n");
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
        client.sendmsg(":ircserv 403 " + client.GetNick() + " " + ch_name + " :No such channel\r\n");
        return;
    }

    if (!chnl->is_member(&client))
    {
        client.sendmsg(":ircserv 442 " + client.GetNick() + " " + ch_name + " :You're not on that channel\r\n");
        return;
    }

    if (!chnl->isoperator(client.GetNick()))
    {
        client.sendmsg(":ircserv 482 " + client.GetNick() + " " + ch_name + " :You're not channel operator\r\n");
        return;
    }

    Client *target = chnl->GetMemberByName(targetNick);
    if (!target)
    {
        client.sendmsg(":ircserv 441 " + client.GetNick() + " " + targetNick + " " + ch_name + " :They aren't on that channel\r\n");
        return;
    }

    std::string kickMsg = ":" + client.GetNick() + "!" + client.GetUsername() +
                          "@" + client.GetIp() + " KICK " + ch_name + " " +
                          targetNick + " :" + reason + "\r\n";
    chnl->broadcast(kickMsg);

    chnl->removeClaint(target);
}

