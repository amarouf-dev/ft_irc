#include "../headers/Server.hpp"

void Server::handle_mode(Client &client, std::vector<std::string> &args)
{
    if (!client.IsAuthenticated())
    {
        std::string reply = Replies::ERR_NOTREGISTERED(serverName, client.GetNick());
        client.sendmsg(reply);
        return;
    }

    if (args.size() < 2)
    {
        std::string reply = Replies::ERR_NEEDMOREPARAMS(serverName, client.GetNick(), "MODE");
        client.sendmsg(reply);
        return;
    }

    std::string target = args[1];

    if (!target.empty() && (target[0] == '#' || target[0] == '&'))
    {
        Channel *chan = getChannel(target);
        if (!chan)
        {
            std::string reply = Replies::ERR_NOSUCHCHANNEL(serverName, client.GetNick(), target);
            client.sendmsg(reply);
            return;
        }

        // if no mode string provided, return current modes
        if (args.size() == 2)
        {
            std::string mode_str = "+";
            std::string params;

            if (chan->GetInviteonly())
                mode_str += "i";
            if (chan->GetTopicRestricted())
                mode_str += "t";
            if (chan->hasKey())
            {
                mode_str += "k";
                params += " " + chan->getKey();
            }
            if (chan->hasUserLimit())
            {
                mode_str += "l";
                std::ostringstream oss;
                oss << chan->getUserLimit();
                params += " " + oss.str();
            }

            std::string reply = Replies::RPL_CHANNELMODEIS(serverName, client.GetNick(), target, mode_str + params);
            client.sendmsg(reply);
            return;
        }

        if (!chan->is_member(&client))
        {
            std::string reply = Replies::ERR_NOTONCHANNEL(serverName, client.GetNick(), target);
            client.sendmsg(reply);
            return;
        }

        if (!chan->isoperator(client.GetNick()))
        {
            std::string reply = Replies::ERR_CHANOPRIVSNEEDED(serverName, client.GetNick(), target);
            client.sendmsg(reply);
            return;
        }

        apply_channel_mode_flags(client, chan, args);
    }
    else
    {

        if (target != client.GetNick())
        {
            std::string reply = Replies::ERR_USERSDONTMATCH(serverName, client.GetNick());
            client.sendmsg(reply);
            return;
        }

        if (args.size() == 2)
        {
            std::string reply = ":" + serverName + " 221 " + client.GetNick() + " +\r\n";
            client.sendmsg(reply);
            return;
        }

        std::string reply = ":" + serverName + " 221 " + client.GetNick() + " +\r\n";
        client.sendmsg(reply);
    }
}

void Server::apply_channel_mode_flags(Client &client, Channel *chan, std::vector<std::string> &args)
{
    std::string mode_string = args[2];
    bool adding = true;
    size_t param_index = 3;

    std::string applied_modes;
    std::vector<std::string> applied_params;

    for (size_t i = 0; i < mode_string.size(); i++)
    {
        char c = mode_string[i];

        if (c == '+')
        {
            adding = true;
            continue;
        }
        else if (c == '-')
        {
            adding = false;
            continue;
        }

        int result = 0;
        switch (c)
        {
            case 'i':
                result = handle_mode_i(chan, adding);
                if (result == 0)
                {
                    if (adding)
                        applied_modes += "+";
                    else
                        applied_modes += "-";
                    applied_modes += std::string(1, c);
                }
                break;
            case 't':
                result = handle_mode_t(chan, adding);
                if (result == 0)
                {
                    if (adding)
                        applied_modes += "+";
                    else
                        applied_modes += "-";
                    applied_modes += std::string(1, c);
                }
                break;
            case 'k':
                result = handle_mode_k(client, chan, args, adding, param_index);
                if (result == 0)
                {
                    if (adding)
                        applied_modes += "+";
                    else
                        applied_modes += "-";
                    applied_modes += std::string(1, c);
                    if (adding && param_index - 1 < args.size())
                        applied_params.push_back(args[param_index - 1]);
                }
                break;
            case 'o':
                result = handle_mode_o(client, chan, args, adding, param_index);
                if (result == 0)
                {
                    if (adding)
                        applied_modes += "+";
                    else
                        applied_modes += "-";
                    applied_modes += std::string(1, c);
                    if (param_index - 1 < args.size())
                        applied_params.push_back(args[param_index - 1]);
                }
                break;
            case 'l':
                result = handle_mode_l(client, chan, args, adding, param_index);
                if (result == 0)
                {
                    if (adding)
                        applied_modes += "+";
                    else
                        applied_modes += "-";
                    applied_modes += std::string(1, c);
                    if (adding && param_index - 1 < args.size())
                        applied_params.push_back(args[param_index - 1]);
                }
                break;
            default:
                client.sendmsg(Replies::ERR_UNKNOWNMODE(serverName, client.GetNick(), c));
                break;
        }
    }

    if (!applied_modes.empty())
        notify_channel_mode_change(client, chan, applied_modes, applied_params);
}

int Server::handle_mode_i(Channel *chan, bool add)
{
    chan->set_invite_only(add);
    return 0;
}

int Server::handle_mode_t(Channel *chan, bool add)
{
    chan->set_topic_restricted(add);
    return 0;
}

int Server::handle_mode_k(Client &client, Channel *chan, std::vector<std::string> &args, 
                          bool add, size_t &index)
{
    if (add)
    {
        if (index >= args.size())
        {
            std::string reply = Replies::ERR_NEEDMOREPARAMS(serverName, client.GetNick(), "MODE");
            client.sendmsg(reply);
            return -1;
        }
        std::string key = args[index++];
        chan->set_key(key, true);
    }
    else
        chan->set_key("", false);
    return 0;
}

int Server::handle_mode_l(Client &client, Channel *chan, std::vector<std::string> &args, 
                          bool add, size_t &index)
{
    if (add)
    {
        if (index >= args.size())
        {
            std::string reply = Replies::ERR_NEEDMOREPARAMS(serverName, client.GetNick(), "MODE");
            client.sendmsg(reply);
            return -1;
        }
        std::string limit_str = args[index++];
        
        std::istringstream iss(limit_str);
        size_t limit;
        if (!(iss >> limit) || limit == 0)
            return -1;
        chan->set_member_limit(limit, true);
    }
    else
        chan->set_member_limit(0, false);
    return 0;
}

int Server::handle_mode_o(Client &client, Channel *chan, std::vector<std::string> &args, 
                          bool add, size_t &index)
{
    if (index >= args.size())
    {
        std::string reply = Replies::ERR_NEEDMOREPARAMS(serverName, client.GetNick(), "MODE");
        client.sendmsg(reply);
        return -1;
    }

    std::string target_nick = args[index++];
    Client *target = FindClaintByName(target_nick);

    if (!target)
    {
        std::string reply = Replies::ERR_NOSUCHNICK(serverName, client.GetNick(), target_nick);
        client.sendmsg(reply);
        return -1;
    }

    if (!chan->is_member(target))
    {
        std::string reply = Replies::ERR_USERNOTINCHANNEL(serverName, client.GetNick(), 
                                                           target_nick, chan->getName());
        client.sendmsg(reply);
        return -1;
    }

    if (add)
        chan->add_operator(target);
    else
        chan->remove_operator(target);

    return 0;
}

void Server::notify_channel_mode_change(Client &client, Channel* chan, const std::string &mode_string, 
                                        const std::vector<std::string> &params)
{
    std::string msg = ":" + client.GetNick() + "!" + client.GetUsername() + 
                      "@" + client.GetIp() + " MODE " + chan->getName() + " " + mode_string;

    for (size_t i = 0; i < params.size(); i++)
        msg += " " + params[i];

    msg += "\r\n";
    chan->broadcast(msg);
    std::cout << GREEN << "Mode change on " << chan->getName() << ": " << mode_string << WHITE << std::endl;
}