#include "../headers/Server.hpp"

int Server::handle_mode_i(Channel* chan, bool add) 
{
    chan->set_invite_only(add);
    return 1;
}

int Server::handle_mode_t(Channel* chan, bool add) 
{
    chan->set_topic_restricted(add);
    return 1;
}

int Server::handle_mode_k(Client &client, Channel* chan, std::vector<std::string>& args, bool add, size_t &index) 
{
    if (add) 
    {
        if (index >= args.size()) 
        {
            sendToClient(client.GetFd(), ":ircserv 461 " + client.GetNick() 
            + " MODE " + chan->getName() + " :Key required for +k mode\r\n");
            return 0;
        }
        chan->set_key(args[index++], true);
    } else {
        chan->set_key("", false);
    }
    return 1;
}

int Server::handle_mode_l(Client &client, Channel* chan, std::vector<std::string>& args, bool add, size_t &index) 
{
    if (add) 
    {
        if (index >= args.size()) 
        {
            sendToClient(client.GetFd(), ":ircserv 461 " + client.GetNick() 
            + " MODE " + chan->getName() + " :Limit required for +l mode\r\n");
            return 0;
        }
        int limit = std::atoi(args[index++].c_str());
        if (limit <= 0) 
        {
            sendToClient(client.GetFd(), ":ircserv 461 " + client.GetNick() 
            + " MODE " + chan->getName() + " :Invalid limit\r\n");
            return 0;
        }
        chan->set_member_limit(limit, true);
    } else {
        chan->set_member_limit(0, false);
    }
    return 1;
}

int Server::handle_mode_o(Client &client, Channel* chan, std::vector<std::string>& args, bool add, size_t &index) 
{
    if (index >= args.size()) {
        sendToClient(client.GetFd(), ":ircserv 461 " + client.GetNick() 
        + " MODE " + chan->getName() + " :Nickname required for +o/-o\r\n");
        return 0;
    }

    std::string nick = args[index++];
    Client* target = FindClaintByName(nick);
    if (!target || !chan->is_client_in_channel(target)) {
        sendToClient(client.GetFd(), ":ircserv 441 " + client.GetNick() + " " 
        + nick + " " + chan->getName() + " :They aren't on that channel\r\n");
        return 0;
    }

    if (add)
        chan->add_operator(target);
    else if (!add && chan->is_operator_in_channel(target))
        chan->remove_operator(target);
    else {
        sendToClient(client.GetFd(), ":ircserv 482 " + client.GetNick() + " " 
        + chan->getName() + " :User is not a channel operator\r\n");
        return 0;
    }
    return 1;
}

void Server::notify_channel_mode_change(Client &client, Channel* chan, 
    const std::string &success_modes, const std::vector<std::string> &success_params) 
{
    std::string notif = ":" + client.GetNick() + "!" + client.GetUsername() + "@" 
    + client.GetIp() + " MODE " + chan->getName() + " " + success_modes;
    for (size_t j = 0; j < success_params.size(); ++j) {
        notif += " " + success_params[j];
    }
    notif += "\r\n";

    const std::set<Client*>& mem = chan->getMembers();
    for (std::set<Client*>::const_iterator it = mem.begin(); it != mem.end(); ++it) {
        sendToClient((*it)->GetFd(), notif);
    }
}

void Server::apply_channel_mode_flags(Client &client, Channel* chan, std::vector<std::string>& args) 
{
    if (args.size() < 3) return;
    std::string flag_str = args[2];
    bool add = true;
    size_t index = 3;

    std::string success_modes;
    std::vector<std::string> success_params;

    for (size_t i = 0; i < flag_str.size(); i++) {
        char flag = flag_str[i];
        if (flag == '+') { add = true; success_modes += "+"; continue; }
        if (flag == '-') { add = false; success_modes += "-"; continue; }

        int success = 0;
        if (flag == 'i')
            success = handle_mode_i(chan, add);
        else if (flag == 't')
            success = handle_mode_t(chan, add);
        else if (flag == 'k') 
        {
            success = handle_mode_k(client, chan, args, add, index);
            if (success) 
            {
                if (add)
                    success_params.push_back(args[index - 1]);
            }
        }
        else if (flag == 'l') 
        {
            success = handle_mode_l(client, chan, args, add, index);
            if (success) 
            {
                if (add)
                    success_params.push_back(args[index - 1]);
            }
        }
        else if (flag == 'o') 
        {
            success = handle_mode_o(client, chan, args, add, index);
            if (success) 
                success_params.push_back(args[index - 1]);
        }
        else 
        {
            sendToClient(client.GetFd(),
                ":" + serverName + " 472 " + client.GetNick() +
                " " + std::string(1, flag) + " :is unknown mode char to me\r\n");
            continue;
        }
        
        if (success) success_modes += flag;
    }

    if (!success_modes.empty())
        notify_channel_mode_change(client, chan, success_modes, success_params);
}

void Server::handle_mode(Client &client, std::vector<std::string>& args) 
{
    if (args.size() < 2) 
    {
        sendToClient(client.GetFd(), ":ircserv 461 " + client.GetNick()
            + " MODE :Not enough parameters\r\n");
        return;
    }

    std::string name = args[1];
    if (name.empty() || (name[0] != '#' && name[0] != '&')) {
        sendToClient(client.GetFd(), ":ircserv 403 " + client.GetNick()
            + " " + name + " :No such channel\r\n");
        return;
    }

    Channel* chan = getChannel(name);
    if (!chan) 
    {
        sendToClient(client.GetFd(), ":ircserv 403 " + client.GetNick()
            + " " + name + " :No such channel\r\n");
        return;
    }

    if (!chan->is_client_in_channel(&client)) 
    {
        sendToClient(client.GetFd(), ":ircserv 442 " + client.GetNick()
            + " " + name + " :You're not on that channel\r\n");
        return;
    }

    if (!chan->is_operator_in_channel(&client)) 
    {
        sendToClient(client.GetFd(), ":ircserv 482 " + client.GetNick()
            + " " + name + " :You're not channel operator\r\n");
        return;
    }

    apply_channel_mode_flags(client, chan, args);
}