#include "../headers/Server.hpp"

void Server::handle_pass(Client &client, const std::vector<std::string> &args)
{
    if (client.IsAuthenticated())
    {
        client.sendmsg("You are already authenticated\r\n");
        return;
    }

    if (args.size() < 2 || args[1].empty())
    {
        client.sendmsg("PASS :Not enough parameters\r\n");
        return;
    }

    const std::string &pass_arg = args[1];

    if (pass_arg == password)
    {
        client.Authenticate();
        client.sendmsg("Password accepted, you are authenticated!\r\n");
        std::cout << GREEN << "Client (" << client.GetFd() << ") authenticated successfully" << WHITE << std::endl;
    }
    else
        client.sendmsg("Password incorrect\r\n");
}