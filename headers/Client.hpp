

#ifndef ClientTT
#define ClientTT

#include "Channel.hpp"
#include <poll.h>
#include <iostream>

class Client;
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

        std::string outbuf;
        pollfd *pfd;

        Channel *cur_chnl;



    public:

        Client();

        int GetFd();
        void SetFd(int);

        std::string GetIp();
        void SetIp(std::string);

        //--------------------------
        bool IsAuthenticated();
        void Authenticate();

        void SetNick(const std::string &nick);
        std::string GetNick() const;

        void SetUsername(const std::string &user);
        std::string GetUsername() const;

        void SetRealname(const std::string &real);
        std::string GetRealname() const;

        void SetBuffer(const std::string&);
        std::string& GetBuffer();
        const std::string& GetBuffer() const;

        void SetPfd(pollfd *pfd);
        pollfd *GetPfd(void) const;

        // Use instead of send:
        void sendmsg(std::string);
};

#endif
