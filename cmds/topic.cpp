

#include "../headers/Server.hpp"

void Server::handle_topic(Client &client, const std::vector<std::string> &args)
{
    if (args.size() == 0) 
    {
        std::string msg = ":ircserv 461 " + client.GetNick() + " TOPIC :Not enough parameters\r\n";
        client.sendmsg(msg);
        return;
    }

    Channel *chan = getChannel(args[0]);
    if (!chan)
    {
        std::string msg = ":ircserv 403 " + client.GetNick() + " " + args[0] + " :No such channel\r\n";
        client.sendmsg(msg);
        return;
    }

    if (!chan->is_member(&client)) 
    {
        std::string msg = ":ircserv 442 " + client.GetNick() + " " + args[0] + " :You're not on that channel\r\n";
        client.sendmsg(msg);
        return;
    }

    if (args.size() == 1)
    {
        if (chan->GetTopic().empty()) 
        {
            std::string msg = ":ircserv 331 " + client.GetNick() + " " + args[0] + " :No topic is set\r\n";
            client.sendmsg(msg);
        }
        else 
        {
            std::string msg = ":ircserv 332 " + client.GetNick() + " " + args[0] + " :" + chan->GetTopic() + "\r\n";
            client.sendmsg(msg);
        }
        return;
    }

    if (!chan->isoperator(client.GetNick()))
    {
        std::string msg = ":ircserv 482 " + client.GetNick() + " " + args[0] + " :You're not channel operator\r\n";
        client.sendmsg(msg);
        return;
    }
    std::string new_topic = args[1];
    for (size_t i = 2; i < args.size(); i++)
        new_topic += " " + args[i];
    chan->SetTopic(new_topic);

    std::string broadcastMsg = ":" + client.GetNick() + "!" + client.GetUsername() + 
                               "@" + client.GetIp() + " TOPIC " + args[0] + " :" + new_topic + "\r\n";
    chan->broadcast(broadcastMsg);
}

