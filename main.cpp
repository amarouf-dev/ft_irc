#include "Server.hpp"

int main (int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>\n";
        return 1;
    }

    int port = std::atoi(av[1]); 
    std::string password = av[2];

    try
    {
        Server s(port, password);
        s.StartServer();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}



// --------------------TEST WITH HEXCHAT 
//  in HexChat type these commands in order:
//
//    /PASS password   <-- must match the server password
//    /NICK nickname
//    /USER username 0 * :realname   <--pay attention to the syntax
//
//  Now you are authenticated u can /JOIN #chan  --> still figuring it out 