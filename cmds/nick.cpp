#include "../headers/Server.hpp"

bool Server::isNickTaken(const std::string &nick)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->GetNick() == nick)
            return true;
    }
    return false;
}

bool Server::isValidNickName(const std::string &str)
{
    if (str.empty() || str.length() > 9 || !isalpha(str[0]))
        return false;

    for (size_t i = 1; i < str.size(); i++)
    {
        if (!isalpha(str[i]) && !isdigit(str[i]) && 
            str[i] != '-' && str[i] != '[' && str[i] != ']' && str[i] != '\\' && 
            str[i] != '`' && str[i] != '_' && str[i] != '^' && str[i] != '{' && 
            str[i] != '}' && str[i] != '|')
            return false;
    }

    return true;
}

void Server::handle_nick(Client &client, const std::vector<std::string> &args)
{
    if (!client.IsAuthenticated())
    {
        std::string reply = Replies::ERR_NOTREGISTERED(serverName, client.GetNick());
        client.sendmsg(reply);
        return;
    }
    else
    {
        if (args.size() < 2 || args[1].empty())
        {
            std::string reply = Replies::ERR_NONICKNAMEGIVEN(serverName, client.GetNick());
            client.sendmsg(reply);
            return;
        }
    
        const std::string &nick_arg = args[1];
    
        if (isNickTaken(nick_arg))
        {
            std::string reply = Replies::ERR_NICKNAMEINUSE(serverName, client.GetNick(), nick_arg);
            client.sendmsg(reply);
            return;
        }
    
        if (!isValidNickName(nick_arg))
        {
            std::string reply = Replies::ERR_ERRONEUSNICKNAME(serverName, client.GetNick(), nick_arg);
            client.sendmsg(reply);
            return;
        }
    
        std::string oldNick = client.GetNick();
        bool wasRegistered = client.IsAuthenticated() && !oldNick.empty() && 
                             !client.GetUsername().empty() && !client.GetRealname().empty();
        client.SetNick(nick_arg);
    
    
        if (wasRegistered)
       {
           std::string msg = ":" + oldNick + "!" + client.GetUsername() + 
                           "@" + client.GetIp() + " NICK :" + nick_arg + "\r\n";
           
           client.sendmsg(msg);
           
           std::set<Client*> notified;
           for (size_t i = 0; i < channels.size(); i++)
           {
               if (channels[i]->is_client_in_channel(&client))
               {
                   const std::set<Client*>& members = channels[i]->getMembers();
                   for (std::set<Client*>::const_iterator it = members.begin(); 
                       it != members.end(); ++it)
                   {
                       if (*it != &client && notified.find(*it) == notified.end())
                       {
                           (*it)->sendmsg(msg);
                           notified.insert(*it);
                       }
                   }
               }
           }
       }
       // ghir if registered for the first time send welcome messages
       else if (client.IsAuthenticated() && !client.GetUsername().empty() && 
                !client.GetRealname().empty())
           welcomeClient(client);
    }
}