

#ifndef SERVERRRR
#define SERVERRRR

#include <set>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sstream> 
#include <poll.h>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <algorithm>
#include "Client.hpp"
#include "Channel.hpp"

#define RED "\e[1;31m"
#define GREEN "\e[1;32m"
#define WHITE "\e[0;37m"
#define YELLO "\e[1;33m"
#define BUFSIZE 1024

class Server
{
    private:
        int port;
        int sockfd;
        std::string password;
        std::vector<Client> clients;
        std::vector<pollfd> poll_fds;
        std::vector<Channel*> channels;

        void CreateSocket();
        void MainLoop();
        void NewClient();
        void NewData(int);
        void removeClient(int);
        void createchannel(std::string, int);

        //--------------------------handle commands
        void handle_pass(Client &client, const std::string &pass_arg);
        void handle_nick(Client &client, const std::string &nick_arg);
        void handle_user(Client &client, const std::vector<std::string> &args);
        void handle_join(Client &client, const std::string &channel_name);

        //--------------------------some helpers
        bool isNickTaken(const std::string &nick);
        bool isValidNickName(const std::string &nick);
        bool isValidUsername(const std::string &username);
        void welcomeClient(Client &client);
        Channel* getOrCreateChannel(const std::string &channel_name);

        // TODO: memory cleanup for channels


    public:
        Server(int port, const std::string &password);
        void StartServer();
};

#endif
