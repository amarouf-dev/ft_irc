#include "../Server.hpp"

void Server::handle_pass(Client &client, const std::string &pass_arg)
{
    if (client.IsAuthenticated())
    {
        std::string rep = "You are already authenticated\r\n";
        send(client.GetFd(), rep.c_str(), rep.size(), 0);
        return;
    }

    if (pass_arg.empty())
    {
        std::string rep = "PASS: Not enough parameters\r\n";
        send(client.GetFd(), rep.c_str(), rep.size(), 0);
        return;
    }

    if (pass_arg == password)
    {
        client.Authenticate();
        std::string rep = "Password accepted, you are authenticated!\r\n";
        send(client.GetFd(), rep.c_str(), rep.size(), 0);
        std::cout << GREEN << "Client (" << client.GetFd() << ") authenticated successfully" << WHITE << std::endl;
    }
    else
    {
        std::string rep = "Password incorrect\r\n";
        send(client.GetFd(), rep.c_str(), rep.size(), 0);
    }
}