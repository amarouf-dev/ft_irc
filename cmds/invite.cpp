
#include "../headers/Server.hpp"

void Server::handle_invite(Client &client, const std::vector<std::string> &args)
{
    if (args.size() < 3)
    {
        std::string reply = Replies::ERR_NEEDMOREPARAMS(serverName, client.GetNick(), "INVITE");
        client.sendmsg(reply);
        return;
    }

    std::string n_name = args[1];
    std::string ch_name = args[2];

    Channel *channel = getChannel(ch_name);
    if (!channel)
    {
        std::string reply = Replies::ERR_NOSUCHCHANNEL(serverName, client.GetNick(), ch_name);
        client.sendmsg(reply);        
        return;
    }

    if (!channel->is_member(&client))
    {
        std::string reply = Replies::ERR_USERONCHANNEL(serverName, client.GetNick(), n_name, ch_name);
        client.sendmsg(reply);       
        return;
    }

    if (channel->GetInviteonly() && !channel->isoperator(client.GetNick()))
    {
        std::string reply = Replies::ERR_CHANOPRIVSNEEDED(serverName, client.GetNick(), ch_name);
        client.sendmsg(reply);        
        return;
    }

    Client *inv_c = FindClaintByName(n_name);
    if (!inv_c)
    {
        std::string reply = Replies::ERR_NOSUCHNICK(serverName, client.GetNick(), n_name);
        client.sendmsg(reply);
        return;
    }

    channel->addClient(inv_c);

    std::string inviteMsg = ":" + client.GetNick() + "!" + client.GetUsername() + 
                            "@" + client.GetIp() + " INVITE " + n_name + " " + ch_name + "\r\n";
    inv_c->sendmsg(inviteMsg);

    std::string confirmMsg = ":ircserv 341 " + client.GetNick() + " " + n_name + " " + ch_name + "\r\n";
    client.sendmsg(confirmMsg);
}

// **************************Issues**************************
// 1 authentication check to verify user is registered first

// 2 check for user already in channel

// 3 incorrect behavior --> adding user directly to channel 
//   currently u're calling channel->addClient() which adds user directly to channel
//    but INVITE should only send an invitation not make the client join directly
//    the user joins later when they run JOIN command

// 4 need to track invitation
// ----> i added this in channel.cpp "std::set<Client*> invited;" 
// ----> i check this list in join.cpp
// ----> u need to add the invited users to this list