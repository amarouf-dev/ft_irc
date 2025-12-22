#ifndef ClientTT
#define ClientTT

#include "Channel.hpp"
#include <poll.h>
#include <iostream>
#include "NumericReplies.hpp"


class Server;
class Channel;

class Client
{
    private:
        int fd;
        std::string ip_addres;   
        bool authenticated;
        std::string nickname;
        std::string username;
        std::string realname;
        std::string buffer;
        std::string outbuf;
        Server* server;

    public:
        Client();
        int GetFd() const;
        std::string GetIp() const;
        std::string GetNick() const;
        std::string GetUsername() const;
        std::string GetRealname() const;
        std::string &GetClientBuffer();
        std::string& GetBuffer();

        void SetFd(int);
        void SetIp(std::string);
        void SetNick(const std::string &nick);
        void SetUsername(const std::string &user);
        void SetRealname(const std::string &real);
        void SetClientBuffer(const std::string &buf);
        void SetBuffer(const std::string&);
        void SetServer(Server* s) { server = s; } 

        bool IsAuthenticated() const;
        void Authenticate();
        void sendmsg(const std::string &msg); 

};

#endif
