
#include "Channel.hpp"

Channel::Channel() {}

void Channel::join_channel(Claint *c)
{
    this->members.insert(c);
}

void Channel::add_op(Claint *c)
{
    this->operators.insert(c);
}


