#ifndef CLIENTTT
#define CLIENTTT

#include <iostream>

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
        void SetUsername(const std::string &user);
        void SetRealname(const std::string &real);
        void SetClientBuffer(std::string buffer);

        
        std::string GetUsername() const;
        std::string GetNick() const;
        std::string GetRealname() const;
        std::string &GetClientBuffer();


};

#endif
