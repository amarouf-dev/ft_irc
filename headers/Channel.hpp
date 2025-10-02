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
        bool inviteonly;
        bool topic_restricted;
        bool has_key;
        std::string key;
        bool has_mem_lim;
        size_t mem_lim;
        
        std::set<Client*> members;
        std::set<Client*> operators;

    public:

        Channel(const std::string &chanName);

        void addClient(Client *client);
        // void removeClient(Client *client);
        void removeClaint(Client *client);

        bool is_member(Client *client);
        bool isoperator(std::string name) const;

        std::string getName() const;
        const std::set<Client*>& getMembers() const;
        Client* GetMemberByName(std::string name) const;
        bool GetInviteonly(void) const;
        const std::string GetTopic() const;

        void SetTopic(std::string);

        // Broadcasts a msg to all claints 
        void broadcast(const std::string &msg);

        // ---------------- MODE
        void set_key(const std::string &key, bool var);
        void set_member_limit(size_t limit, bool var);
        void add_operator(Client *client);
        void remove_operator(Client *client);
        void set_invite_only(bool var);
        void set_topic_restricted(bool var);
        bool is_client_in_channel(Client *client) const;
        bool is_operator_in_channel(Client *client) const;
};

#endif
