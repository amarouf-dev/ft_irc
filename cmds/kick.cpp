
#include "../headers/Server.hpp"

void Server::handle_kick(Client &client, const std::vector<std::string> &args)
{
    Channel *chnl = client.GetCurChannel();
    Client *c = NULL;

    if (chnl == NULL)
    {
        std::cout << RED << "No channel was found!" << WHITE << std::endl;
        return ;
    }
    
    if (!client.IsAuthenticated())
    {
        std::string msg = "You must authenticate first!\r\n";
        send(client.GetFd(), msg.c_str(), msg.size(), 0);
        return;
    }
    if (args.size() == 3)
    {
        for (std::set<Client *>::iterator it = chnl->getMembers().begin(); it != chnl->getMembers().end(); it++)
        {
            if ((*it)->GetNick() == args[1])
            {
                c = *it;
                break;
            }
        }
        if (c == NULL)
        {
            std::cout << RED << "The user (" << args[1] << ") was not found !" << WHITE << std::endl;
            return ;
        }
        chnl->removeClaint(c);
    }
}
