
#include "../headers/Channel.hpp"

Channel::Channel(const std::string &chanName) : name(chanName) {}

// void Channel::join_channel(Client *c)
// {
// 	if (this->members.find(c) == members.end())
// 	{
// 		std::cout << YELLO << c->GetFd() << " Joined the channel " << this->name << WHITE << std::endl;
// 		this->members.insert(c);
// 	}
// 	else
//    	{
// 		std::cout << RED << c->GetFd() << " already in the channel !" << WHITE << std::endl;
// 	}
// }

// void Channel::add_op(Client *c)
// {
// 	this->operators.insert(c);
// }

// void Channel::Initchannel(std::string n, Client *c)
// {
// 	this->name = n;
// 	this->members.insert(c);
// 	std::cout << GREEN << "Channel " << name << " Created !" << WHITE << std::endl;
// }

void Channel::addClient(Client *client)
{
    // for (size_t i = 0; i < members.size(); i++)
    // {
    //     if (members[i] == client)
    //         return;
    // }
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

const Client* Channel::GetMemberByName(std::string name) const
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




