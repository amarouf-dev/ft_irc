#include "../headers/Channel.hpp"

Channel::Channel(const std::string &chanName) 
    : name(chanName), topic(""), inviteonly(false), topic_restricted(false),     
    has_key(false), key(""), has_mem_lim(false), mem_lim(0) {}
 
void Channel::addClient(Client *client)
{
    if (members.find(client) != members.end())
        return;
    
    members.insert(client);
    if (members.size() == 1)
        operators.insert(client);
    
    if (invited.find(client) != invited.end())
        invited.erase(client);
}

void Channel::removeClaint(Client *client)
{
    members.erase(client);
    operators.erase(client);
    invited.erase(client); 

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
    Client* client = GetMemberByName(name);
    if (!client)
        return (false);
    return operators.find(client) != operators.end();
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

bool Channel::GetTopicRestricted() const
{
    return topic_restricted;
}

bool Channel::hasUserLimit() const 
{
    return has_mem_lim;
}

size_t Channel::getUserLimit() const 
{
    return mem_lim;
}

bool Channel::hasKey() const 
{
    return has_key;
}

std::string Channel::getKey() const 
{
    return key;
}


bool Channel::is_invited(Client *client) const
{
    return invited.find(client) != invited.end();
}

void Channel::remove_invited(Client *client)
{
    invited.erase(client);
}

void Channel::add_invited(Client *client)
{
    this->invited.insert(client);
}
