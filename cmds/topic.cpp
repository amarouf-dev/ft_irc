
#include "../headers/Server.hpp"

void Server::handle_topic(Client &client, const std::vector<std::string> &args)
{
    if (args.size() < 2) 
    {
        std::string msg = Replies::ERR_NEEDMOREPARAMS(serverName, client.GetNick(), "TOPIC");
        client.sendmsg(msg);
        return;
    }
    
    if (!client.IsAuthenticated())
    {
        std::string reply = Replies::ERR_NOTREGISTERED(serverName, client.GetNick());
        client.sendmsg(reply);
        return;
    }

    Channel *chan = getChannel(args[1]);
    if (!chan)
    {
        std::string msg = Replies::ERR_NOSUCHCHANNEL(serverName, client.GetNick(), args[1]);
        client.sendmsg(msg);
        return;
    }

    if (!chan->is_member(&client)) 
    {
        std::string msg = Replies::ERR_NOTONCHANNEL(serverName, client.GetNick(), args[1]);
        client.sendmsg(msg);
        return;
    }

    if (args.size() == 2)
    {
        if (chan->GetTopic().empty()) 
        {
            std::string msg = ":ircserv 331 " + client.GetNick() + " " + args[1] + " :No topic is set\r\n";
            client.sendmsg(msg);
        }
        else 
        {
            std::string msg = ":ircserv 332 " + client.GetNick() + " " + args[1] + " :" + chan->GetTopic() + "\r\n";
            client.sendmsg(msg);
        }
        return;
    }

    if (chan->GetTopicRestricted() && !chan->isoperator(client.GetNick()))
    {
        std::string msg = Replies::ERR_CHANOPRIVSNEEDED(serverName, client.GetNick(), args[1]);
        client.sendmsg(msg);
        return;
    }

    std::string new_topic = args[2];
    if (!new_topic.empty() && new_topic[0] == ':')
        new_topic.erase(0, 1);
    for (size_t i = 3; i < args.size(); i++)
        new_topic += " " + args[i];
    chan->SetTopic(new_topic);

    std::string broadcastMsg = ":" + client.GetNick() + "!" + client.GetUsername() + 
                               "@" + client.GetIp() + " TOPIC " + args[1] + " :" + new_topic + "\r\n";
    chan->broadcast(broadcastMsg);
}
