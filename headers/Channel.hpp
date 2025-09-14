
#include "Server.hpp"


#ifndef CHNNNLL_
#define CHNNNLL_

class Channel
{
    private:
    std::string name;
    std::string topic;

    std::set<Claint *> members;
    std::set<Claint *> operators;

    public:
    Channel();
    void Initchannel(std::string, Claint*);
    void join_channel(Claint*);
    void add_op(Claint*);
    std::string GetName();
};

#endif
