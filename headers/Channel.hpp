

#ifndef CHNNNLL_
#define CHNNNLL_

#include "Server.hpp"
#include "Client.hpp"

class Client;
class Channel;

class Channel
{
    private:

    bool inviteonly;

    std::string name;
    std::string topic;

    std::set<Client *> members;
    std::set<Client *> operators;

    public:
    Channel(const std::string &chanName);

    void addClient(Client *client);
    void removeClaint(Client *client);
    bool is_member(Client *client);
    std::string getName() const;
    const std::set<Client*>& getMembers() const;
    Client* GetMemberByName(std::string name) const;
    bool isoperator(std::string name) const;

    bool GetInviteonly(void) const;

    const std::string GetTopic() const;
    void SetTopic(std::string);

    void broadcast(const std::string &msg);
};

#endif
