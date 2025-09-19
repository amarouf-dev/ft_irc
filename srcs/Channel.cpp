
#include "../headers/Channel.hpp"

Channel::Channel(const std::string &chanName) : name(chanName) {}

void Channel::addClient(Client *client)
{
	if (members.find(client) != members.end())
		return ;
    members.insert(client);
    // TODO: notify everyone about join
    // TODO: make sure if check max members needed? 
}



void Channel::removeClaint(Client *client)
{
    members.erase(client);
	std::cout << YELLO << "The claint " << client->GetNick() << " was removed !" << WHITE << std::endl;
}

std::string Channel::getName() const
{
    return name;
}

bool Channel::is_member(Client *client)
{
	if(members.find(client) != members.end())
		return (true);
	else
		return (false);
}

const std::set<Client*>&  Channel::getMembers() const
{
	return (this->members);
}

Client* Channel::GetMemberByName(std::string name) const
{
    for (std::set<Client*>::iterator it = members.begin(); it != members.end(); it ++)
    {
        if (name == (*it)->GetNick())
        {
            return (*it);
        }
    }
    return (NULL);
}


const std::string Channel::GetTopic() const
{
    return (topic);
}

void Channel::SetTopic(std::string n)
{
    topic = n;
}

bool Channel::isoperator(std::string name) const
{
    for (std::set<Client*>::iterator it = operators.begin(); it != operators.end(); it ++)
    {
        if (name == (*it)->GetNick())
        {
            return (true);
        }
    }
    return (false);
}

void Channel::broadcast(const std::string &msg)
{
    for (std::set<Client*>::iterator it = members.begin(); it != members.end(); it++)
    {
        (*it)->sendmsg(msg);
    }
}

bool Channel::GetInviteonly(void) const
{
    return (inviteonly);
}


