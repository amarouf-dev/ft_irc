

#include "Claint.hpp"


Claint::Claint(){}

int Claint::GetFd()
{
    return (fd);
}

void Claint::SetFd(int n)
{
    fd = n;
}

std::string Claint::GetIp()
{
    return (ip_addres);
}

void Claint::SetIp(std::string n)
{
    ip_addres = n;
}

