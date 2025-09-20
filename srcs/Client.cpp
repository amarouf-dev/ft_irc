

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

void Client::SetBuffer(const std::string& msg)
{
    this->outbuf += msg;
}

void Client::SetPfd(pollfd *npfd)
{
    this->pfd = npfd;
}

pollfd *Client::GetPfd(void) const
{
    return (this->pfd);
}

std::string& Client::GetBuffer()
{
    return outbuf;
}

const std::string& Client::GetBuffer() const
{
    return outbuf;
}

void Client::sendmsg(std::string msg)
{
    this->outbuf += msg;
    this->pfd->events |= POLLOUT;
}

