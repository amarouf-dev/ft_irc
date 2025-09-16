

#ifndef ClientTT
#define ClientTT


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

};

#endif
