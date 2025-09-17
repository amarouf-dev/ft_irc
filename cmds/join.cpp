#include "../headers/Server.hpp"


void Server::handle_join(Client &client, const std::string &channel_name)
{
    std::cout << "DEBUG: Client nick is '" << client.GetNick() << "'" << std::endl;

    
    if (!client.IsAuthenticated())
    {
        std::string msg = "You must authenticate first!\r\n";
        // send(client.GetFd(), msg.c_str(), msg.size(), 0);
        client.SetBuffer(msg);
        client.GetPfd()->events |= POLLOUT;
        return;
    }

    //TODO: handle #
    if (channel_name.empty() || channel_name[0] != '#')
    {
        std::string msg = "Invalid channel name. Must start with #\r\n";
        // send(client.GetFd(), msg.c_str(), msg.size(), 0);
        return;
    }

    Channel* chan = getOrCreateChannel(channel_name);

    chan->addClient(&client);
    client.SetCurChannel(chan);

    std::string msg = ":" + client.GetNick() + " JOIN " + channel_name + "\r\n";
    client.SetBuffer(msg);

    client.GetPfd()->events |= POLLOUT;

    // //later
    // for (size_t i = 0; i < chan->getMembers().size(); i++)
    // {
    //     Client* c = chan->getMembers()[i];
    //     if (c->GetFd() != client.GetFd())
    //     {
    //         std::string join_msg = ":" + client.GetNick() + " JOIN " + channel_name + "\r\n";
    //         send(c->GetFd(), join_msg.c_str(), join_msg.size(), 0);
    //     }
    // }

    std::cout << GREEN << client.GetNick() << " joined " << channel_name << WHITE << std::endl;
}

Channel* Server::getOrCreateChannel(const std::string &channel_name)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i]->getName() == channel_name)
            return channels[i];
    }

    // make new one
    Channel* new_chan = new Channel(channel_name);
    channels.push_back(new_chan);
    return new_chan;
}
