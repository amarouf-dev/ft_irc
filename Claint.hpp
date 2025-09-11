

#ifndef CLAINTTT
#define CLAINTTT


#include <iostream>

class Claint
{
    private:

    int fd;
    std::string ip_addres;

    public:

    Claint();

    int GetFd();
    void SetFd(int);

    std::string GetIp();
    void SetIp(std::string);
};

#endif
