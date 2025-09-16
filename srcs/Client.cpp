

#include "../headers/Client.hpp"

Client::Client() : authenticated(false) {}

int Client::GetFd()
{
    return (fd);
}

void Client::SetFd(int n)
{
    fd = n;
}

std::string Client::GetIp()
{
    return (ip_addres);
}

void Client::SetIp(std::string n)
{
    ip_addres = n;
}

//------------------------------
bool Client::IsAuthenticated()
{
    return authenticated;
}

void Client::Authenticate()
{
    authenticated = true;
}

void Client::SetNick(const std::string &nick)
{ 
    nickname = nick; 
}

std::string Client::GetNick() const 
{ 
    return nickname; 
}

void Client::SetUsername(const std::string &user) 
{ 
    username = user; 
}

std::string Client::GetUsername() const 
{ 
    return username; 
}

void Client::SetRealname(const std::string &real) 
{ 
    realname = real; 
}

std::string Client::GetRealname() const 
{ 
    return realname; 
}

