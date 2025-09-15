#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>
#include <string>

class Channel
{
    private:
        std::string name;
        std::vector<Client*> members;

    public:
        Channel(const std::string &chanName);

        std::string getName() const;
        const std::vector<Client*>& getMembers() const;

        void addClient(Client *client);
        // TODO: removeClient() 
};

#endif
