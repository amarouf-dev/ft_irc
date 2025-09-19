
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
        client.sendmsg(msg);
        return;
    }
    if (args.size() == 3)
    {
        c = chnl->GetMemberByName(args[1]);
        if (c == NULL)
        {
            std::cout << RED << "The user (" << args[1] << ") was not found !" << WHITE << std::endl;
            return ;
        }
        chnl->removeClaint(c);
    }
}
