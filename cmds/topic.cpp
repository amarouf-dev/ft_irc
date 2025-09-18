

#include "../headers/Server.hpp"

void Server::handle_topic(Client &client, const std::vector<std::string> &args)
{

    if (args.size() == 0)
    {
        std::string msg = ":ircserv 461 " + client.GetNick() + " TOPIC :Not enough parameters\r\n";
        send(client.GetFd(), msg.c_str(), msg.size(), 0);
        return;
    }
    if (!client.GetCurChannel())
    {
        send(client.GetFd(), "No channel", 11, 0);
        return ;
    }
    if (args.size() == 1)
    {
        std::string topic = client.GetCurChannel()->GetTopic();
        send(client.GetFd(), topic.c_str(), topic.size(), 0);
    }
    else if (args.size() == 2)
    {
        std::string topic = "The channel's tpic is now set to " + args[1];
        client.GetCurChannel()->SetTopic(args[1]);
        send(client.GetFd(), topic.c_str(), topic.size(), 0);
    }
    else
    {
        std::string topic = "TOPIC :Wrong number of parameters";
        send(client.GetFd(), topic.c_str(), topic.size(), 0);
    }
}
