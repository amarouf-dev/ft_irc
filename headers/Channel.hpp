
#include "Server.hpp"


class Channel
{
    private:
    std::string name;
    std::string topic;

    std::set<Claint *> members;
    std::set<Claint *> operators;

    public:
    Channel();
    void join_channel(Claint*);
    void add_op(Claint*);
};
