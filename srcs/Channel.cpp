
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
		return (false);
	else
		return (true);
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




