
#include "../headers/Channel.hpp"

Channel::Channel(const std::string &chanName) : name(chanName) {}

void Channel::addClient(Client *client)
{
	if (members.find(client) != members.end())
		return ;
    members.insert(client);
    if (members.size() == 1)
        operators.insert(client);
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
            return (*it);

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
    if (GetMemberByName(name))
        return (true);
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

void Channel::set_key(const std::string &k, bool var) {
    has_key = var;
    key = k;
}

void Channel::set_member_limit(size_t limit, bool var) {
    has_mem_lim = var;
    mem_lim = limit;
}

void Channel::add_operator(Client *client) {
    operators.insert(client);
}

void Channel::remove_operator(Client *client) {
    operators.erase(client);
}

void Channel::set_invite_only(bool var) {
    inviteonly = var;
}

void Channel::set_topic_restricted(bool var) {
    topic_restricted = var;
}

bool Channel::is_client_in_channel(Client *client) const {
    return members.find(client) != members.end();
}

bool Channel::is_operator_in_channel(Client *client) const {
    return operators.find(client) != operators.end();
}
