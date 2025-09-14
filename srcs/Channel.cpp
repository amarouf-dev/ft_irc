
#include "../headers/Channel.hpp"

Channel::Channel() {}

void Channel::join_channel(Claint *c)
{
	if (this->members.find(c) == members.end())
	{
		std::cout << YELLO << c->GetFd() << " Joined the channel " << this->name << WHITE << std::endl;
		this->members.insert(c);
	}
	else
   	{
		std::cout << RED << c->GetFd() << " already in the channel !" << WHITE << std::endl;
	}
}

void Channel::add_op(Claint *c)
{
	this->operators.insert(c);
}

void Channel::Initchannel(std::string n, Claint *c)
{
	this->name = n;
	this->members.insert(c);
	std::cout << GREEN << "Channel " << name << " Created !" << WHITE << std::endl;
}

std::string Channel::GetName()
{
	return (name);
}



