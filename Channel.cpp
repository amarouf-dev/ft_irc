#include "includes/Channel.hpp"


Channel::Channel(const std::string &chanName) : name(chanName) {}

std::string Channel::getName() const
{
    return name;
}

const std::vector<Client*>& Channel::getMembers() const
{
    return members;
}

void Channel::addClient(Client *client)
{
    for (size_t i = 0; i < members.size(); i++)
    {
        if (members[i] == client)
            return;
    }
    members.push_back(client);
    // TODO: notify everyone about join
    // TODO: make sure if check max members needed? 
}
