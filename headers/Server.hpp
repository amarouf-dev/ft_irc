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
#include <cstdlib>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <string>
#include <iostream>

#include "Client.hpp"
#include "Channel.hpp"

#define RED "\e[1;31m"
#define GREEN "\e[1;32m"
#define WHITE "\e[0;37m"
#define YELLO "\e[1;33m"
#define BUFSIZE 1024

class Client;
class Channel;

class Server
{
    private:
        int port;
        int sockfd;
        std::string password;
        std::string serverName;

        // std::vector<Client> clients;
        //! 
        std::vector<Client*> clients;
        std::vector<pollfd> poll_fds;
        std::vector<Channel*> channels;


        void CreateSocket();
        void MainLoop();
        void NewClient();
        void NewData(int);
        void removeClient(int);

        //--------------------------handle commands
        void handle_pass(Client &client, const std::vector<std::string> &args);
        void handle_nick(Client &client, const std::vector<std::string> &args);
        void handle_user(Client &client, const std::vector<std::string> &args);
        void handle_join(Client &client, const std::vector<std::string> &args);
        void handle_privmsg(Client &client, const std::vector<std::string> &args);
        void handle_mode(Client &client, std::vector<std::string> &args);

        void handle_kick(Client &client, const std::vector<std::string> &args);
        void handle_topic(Client &client, const std::vector<std::string> &args);
        void handle_invite(Client &client, const std::vector<std::string> &args);

        //--------------------------some helpers
        bool isNickTaken(const std::string &nick);
        bool isValidNickName(const std::string &nick);
        bool isValidUsername(const std::string &username);
        void welcomeClient(Client &client);
    
        //--------------------------Channel
        Channel* getOrCreateChannel(const std::string &channel_name);
        Channel* getChannel(const std::string &channel_name);

        //--------------------------Client
        Client *FindClaintByFd(int fd);
        Client *FindClaintByName(std::string name);

    public:
        Server(int port, const std::string &password);
        ~Server();
        void StartServer();
        void cleanUp();


        void executeCmd(Client& client, const std::string& cmd);
        std::vector<std::string> get_arg(std::string cmd);
        std::string trim(const std::string& str);
        std::string &cmdToUppercase(std::string &str);

        //--------------------------privmsg
        std::string joinMessageArgs(const std::vector<std::string> &args, size_t start);
        std::string buildIrcMessage(const Client &client, const std::string &target, const std::string &message);
        Channel* findChannelByName(const std::string &name);
        Client* findClientByNick(const std::string &nick);
        void sendToChannel(Channel* chan, const Client &sender, const std::string &message);
        void sendToUser(Client* target, const std::string &message);

        //--------------------------MODE
        int handle_mode_i(Channel *chan, bool add);
        int handle_mode_t(Channel *chan, bool add);
        int handle_mode_k(Client &client, Channel *chan, std::vector<std::string> &args, bool add, size_t &index);
        int handle_mode_l(Client &client, Channel *chan, std::vector<std::string> &args, bool add, size_t &index);
        int handle_mode_o(Client &client, Channel *chan, std::vector<std::string> &args, bool add, size_t &index);
        void apply_channel_mode_flags(Client &client, Channel *chan, std::vector<std::string> &args);
        void notify_channel_mode_change(Client &client, Channel *chan, const std::string &modes, const std::vector<std::string> &params);

        void enableWriteEvent(int fd); //!
};

#endif
