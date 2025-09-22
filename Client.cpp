
#include "Client.hpp"


Client::Client() : authenticated(false) {}

//--------------Setters
void Client::SetFd(int n) 
{ 
    fd = n; 
}
void Client::SetIp(std::string n) 
{
    ip_addres = n; 
}
void Client::SetNick(const std::string &nick) 
{ 
    nickname = nick; 
}
void Client::SetUsername(const std::string &user) 
{ 
    username = user; 
}
void Client::SetRealname(const std::string &real) 
{ 
    realname = real; 
}
void Client::SetClientBuffer(std::string buffer) 
{ 
    buffer = buffer; 
}

//--------------Getters
int         Client::GetFd() { return (fd); }
std::string Client::GetIp() { return (ip_addres); }
std::string Client::GetNick() const { return nickname; }
std::string Client::GetUsername() const { return username; }
std::string Client::GetRealname() const { return realname; }
std::string &Client::GetClientBuffer() { return buffer; }


//------------------------------
bool Client::IsAuthenticated() { return authenticated; }
void Client::Authenticate() { authenticated = true; }




