#include "../headers/Server.hpp"


// void Server::sendToClient(int cFd, const std::string& message) 
// {
//     if (cFd == -1)
//         return;
//     std::string msg = message;
//     if (msg.substr(msg.size() - 2) != "\r\n")
//         msg += "\r\n";
//     if (send(cFd, msg.c_str(), msg.length(), 0) == -1)
//         std::cout << "Send failed " << std::endl;
// }

std::string &Server::cmdToUppercase(std::string &str)
{
    for (size_t i = 0; i < str.size(); i++)
        str[i] = toupper(str[i]);
    return str;
}

std::string Server::trim(const std::string& str)
{
    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start])))
        start++;

    size_t end = str.size();
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1])))
        end--;

    return str.substr(start, end - start);
}

std::vector<std::string> Server::get_arg(std::string cmd)
{
    std::vector<std::string> args;
    size_t i = 0;

    cmd = trim(cmd);

    while (i < cmd.size())
    {
        while (i < cmd.size() && (cmd[i] == ' ' || cmd[i] == '\t'))
            i++;

        if (i >= cmd.size())
            break;
        if (cmd[i] == ':')
        {
            if (i < cmd.size())
                args.push_back(cmd.substr(i));
            break;
        }

        size_t j = i;
        while (j < cmd.size() && cmd[j] != ' ' && cmd[j] != '\t' && cmd[j] != ':')
            j++;
        if (i != j)
            args.push_back(cmd.substr(i, j - i));
        i = j;
    }

    return args;
}

void Server::executeCmd(Client& client, const std::string& cmd)
{
    if (cmd.empty())
        return;

    std::vector<std::string> args = get_arg(cmd);
    if (args.empty())
        return;

    args[0] = cmdToUppercase(args[0]);

    if (args[0] == "PASS")
        handle_pass(client, args);
    else if (args[0] == "NICK")
        handle_nick(client, args);
    else if (args[0] == "USER")
        handle_user(client, args);
    else if (args[0] == "JOIN")
        handle_join(client, args);
    else if (args[0] == "PRIVMSG")
        handle_privmsg(client, args);
    else if (args[0] == "MODE")
        handle_mode(client, args);
    else if (args[0] == "INVITE")
        handle_invite(client, args);
    else if (args[0] == "TOPIC")
        handle_topic(client, args);
    else if (args[0] == "KICK")
        handle_kick(client, args);
    else if (args[0] == "CAP" || args[0] == "PING"
          || args[0] == "PONG" || args[0] == "WHO" || args[0] == "QUIT")
        return;
    else
        client.sendmsg("Unknown command\n");
}
