

#include "../headers/Client.hpp"

Client::Client() : authenticated(false) {}


int Client::GetFd() const { return (fd);}
std::string Client::GetIp() const { return (ip_addres); }
std::string Client::GetNick() const { return nickname; }
std::string Client::GetUsername() const { return username; }
std::string Client::GetRealname() const { return realname; }
std::string &Client::GetClientBuffer() { return buffer; }  //  mutable
std::string& Client::GetBuffer() { return outbuf; }
pollfd *Client::GetPfd(void) const { return (this->pfd); }


void Client::SetFd(int n) { fd = n; }
void Client::SetIp(std::string n) { ip_addres = n; }
void Client::SetNick(const std::string &nick) { nickname = nick; }
void Client::SetUsername(const std::string &user) { username = user; }
void Client::SetRealname(const std::string &real) { realname = real; }
void Client::SetClientBuffer(const std::string &buf) { buffer = buf; }
void Client::SetBuffer(const std::string& msg) { this->outbuf += msg; }
void Client::SetPfd(pollfd *npfd) { this->pfd = npfd; }



void Client::sendmsg(std::string msg)
{
    this->outbuf += msg;
    this->pfd->events |= POLLOUT;
}

//------------------------------
bool Client::IsAuthenticated() const { return authenticated; }
void Client::Authenticate() { authenticated = true; }
