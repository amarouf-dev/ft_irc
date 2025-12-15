#include "../headers/Server.hpp"

std::string Server::joinMessageArgs(const std::vector<std::string> &args, size_t start) 
{
    std::string message;
    for (size_t i = start; i < args.size(); ++i) 
    {
        if (i > start)
            message += " ";
        message += args[i];
    }
    if (!message.empty() && message[0] == ':')
        message = message.substr(1);
    return message;
}

std::string Server::buildIrcMessage(const Client &client, const std::string &target, const std::string &message) 
{
    return ":" + client.GetNick() + "!" + client.GetUsername() + "@" +
           client.GetIp() + " PRIVMSG " + target + " :" + message + "\r\n";
}

Channel* Server::findChannelByName(const std::string &name) 
{
    for (size_t i = 0; i < channels.size(); i++) 
    {
        if (channels[i]->getName() == name)
            return channels[i];
    }
    return NULL;
}

Client* Server::findClientByNick(const std::string &nick) 
{
    for (size_t i = 0; i < clients.size(); i++) 
    {
        if (clients[i]->GetNick() == nick)
            return clients[i];
    }
    return NULL;
}

void Server::sendToChannel(Channel* chan, const Client &sender, const std::string &message) 
{
    const std::set<Client*>& members = chan->getMembers();
    for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it) 
    {
        Client* m = *it;
        if (m && m->GetFd() != sender.GetFd())
            m->sendmsg(message);
    }
}


void Server::sendToUser(Client* target, const std::string &message) 
{
    if (target)
        target->sendmsg(message);
}

void Server::handle_privmsg(Client &client, const std::vector<std::string> &args) 
{
    if (!client.IsAuthenticated()) 
    {
        std::string reply = Replies::ERR_NOTREGISTERED(serverName, client.GetNick());
        client.sendmsg(reply);
        return;
    }

    if (args.size() < 2 || args[1].empty())
    {
        std::string reply = Replies::ERR_NORECIPIENT(serverName, client.GetNick(), "PRIVMSG");
        client.sendmsg(reply);
        return;
    }

    if (args.size() < 3 || args[2].empty())
    {
        std::string reply = Replies::ERR_NOTEXTTOSEND(serverName, client.GetNick());
        client.sendmsg(reply);
        return;
    }

    std::string target = args[1];
    std::string message = joinMessageArgs(args, 2);
    std::string fullMsg = buildIrcMessage(client, target, message);

    if (!target.empty() && (target[0] == '#' || target[0] == '&')) 
    {
        Channel* chan = findChannelByName(target);
        if (!chan) 
        {
            std::string reply = Replies::ERR_NOSUCHCHANNEL(serverName, client.GetNick(), target);
            client.sendmsg(reply);            
            return;
        }
        sendToChannel(chan, client, fullMsg);
    } 
    else 
    {
        Client* targetClient = findClientByNick(target);
        if (!targetClient) 
        {
            std::string reply = Replies::ERR_NOSUCHNICK(serverName, client.GetNick(), target);
            client.sendmsg(reply);
            return;
        }
        sendToUser(targetClient, fullMsg);
    }
}
