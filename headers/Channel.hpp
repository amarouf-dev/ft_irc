

#ifndef CHNNNLL_
#define CHNNNLL_

#include "Server.hpp"
#include "Client.hpp"

class Client;
class Channel;

class Channel
{
    private:
    std::string name;
    std::string topic;

    std::set<Client *> members;
    std::set<Client *> operators;

    public:
    Channel(const std::string &chanName);
    // void Initchannel(std::string, Client*);
    // void join_channel(Client*);
    void addClient(Client *client);
    void removeClaint(Client *client);
    bool is_member(Client *client);
    // void add_op(Client*);
    std::string getName() const;
    const std::set<Client*>& getMembers() const;
    const Client* GetMemberByName(std::string name) const;
};

// #ifndef CHANNEL_HPP
// #define CHANNEL_HPP

// #include "Client.hpp"
// #include <vector>
// #include <string>

// class Channel
// {
//     private:
//         std::string name;
//         std::vector<Client*> members;

//     public:
//         Channel(const std::string &chanName);

//         std::string getName() const;
//         const std::vector<Client*>& getMembers() const;

//         // TODO: removeClient() 
// };

#endif
