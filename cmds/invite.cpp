
#include "../headers/Server.hpp"

void Server::handle_invite(Client &client, const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        client.sendmsg(":ircserv 461 " + client.GetNick() + " INVITE :Not enough parameters\r\n");
        return;
    }

    std::string n_name = args[0];
    std::string ch_name = args[1];

    Channel *channel = getChannel(ch_name);
    if (!channel)
    {
        client.sendmsg(":ircserv 403 " + client.GetNick() + " " + ch_name + " :No such channel\r\n");
        return;
    }

    if (!channel->is_member(&client))
    {
        client.sendmsg(":ircserv 442 " + client.GetNick() + " " + ch_name + " :You're not on that channel\r\n");
        return;
    }

    // if (channel->GetInviteonly() && !channel->is_operator_in_channel(&client))
    if (channel->GetInviteonly() && !channel->isoperator(client.GetNick()))
    {
        client.sendmsg(":ircserv 482 " + client.GetNick() + " " + ch_name + " :You're not channel operator\r\n");
        return;
    }

    Client *inv_c = FindClaintByName(n_name);
    if (!inv_c)
    {
        client.sendmsg(":ircserv 401 " + client.GetNick() + " " + n_name + " :No such nick\r\n");
        return;
    }

    channel->addClient(inv_c);

    std::string inviteMsg = ":" + client.GetNick() + "!" + client.GetUsername() + 
                            "@" + client.GetIp() + " INVITE " + n_name + " " + ch_name + "\r\n";
    inv_c->sendmsg(inviteMsg);

    std::string confirmMsg = ":ircserv 341 " + client.GetNick() + " " + n_name + " " + ch_name + "\r\n";
    client.sendmsg(confirmMsg);
}
