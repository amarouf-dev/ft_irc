
#include "../headers/Server.hpp"

void Server::handle_topic(Client &client, const std::vector<std::string> &args)
{
    if (args.size() == 0) 
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
    if (!new_topic.empty())
        new_topic.erase(0, 1);
    std::cout << RED << new_topic << WHITE << std::endl;
    for (size_t i = 3; i < args.size(); i++)
        new_topic += " " + args[i];
    chan->SetTopic(new_topic);

    std::string broadcastMsg = ":" + client.GetNick() + "!" + client.GetUsername() + 
                               "@" + client.GetIp() + " TOPIC " + args[1] + " :" + new_topic + "\r\n";
    chan->broadcast(broadcastMsg);
}

// **************************Issues**************************
// 1 authentication check to verify user is registered first // DONE

// 2 missing Channel Parameter not handled

// 3 channel name apears in topic with this test "/topic #chan" 
// test : /topic #chan Hello world
// ---> expected Behavior: Topic for #chan :Hello World
// ---> current behavior:  Topic for #chan :#chan Hello World // DONE

// 4 handling :
// test : /topic #chan :Hello world
// ---> expected Behavior: Topic for #chan is: Hello world
// ---> current behavior:  Topic for #chan is: :Hello world // DONE

// 5 when channel mode +t is OFF ( /mode #chan -t ) the IRC reference say any user can change the channel topic even non-operator
// in this test a regular user should be allowed to set the topic
// our code always requires operator privileges, even when +t is not set // DONE 





