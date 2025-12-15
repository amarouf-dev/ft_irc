#include "../headers/NumericReplies.hpp"

// Welcome messages
std::string Replies::RPL_WELCOME(const std::string& serverName, const std::string& nick, 
                                 const std::string& user, const std::string& host)
{
    std::string msg = ":" + serverName + " 001 " + nick;
    msg += " :Welcome to the IRC network " + nick + "!" + user + "@" + host + "\r\n";
    return msg;
}

std::string Replies::RPL_YOURHOST(const std::string& serverName, const std::string& nick)
{
    std::string msg = ":" + serverName + " 002 " + nick;
    msg += " :Your host is " + serverName + ", running version 1.0\r\n";
    return msg;
}

std::string Replies::RPL_CREATED(const std::string& serverName, const std::string& nick)
{
    std::string msg = ":" + serverName + " 003 " + nick;
    msg += " :This server was created just now\r\n";
    return msg;
}

std::string Replies::RPL_MYINFO(const std::string& serverName, const std::string& nick)
{
    std::string msg = ":" + serverName + " 004 " + nick + " ";
    msg += serverName + " 1.0 io itklno\r\n";
    return msg;
}

// Channel replies
std::string Replies::RPL_NAMREPLY(const std::string& serverName, const std::string& nick, 
                                 const std::string& channel, const std::string& names)
{
    std::string msg = ":" + serverName + " 353 " + nick;
    msg += " = " + channel + " :" + names + "\r\n";
    return msg;
}

std::string Replies::RPL_ENDOFNAMES(const std::string& serverName, const std::string& nick, 
                                   const std::string& channel)
{
    std::string msg = ":" + serverName + " 366 " + nick;
    msg += " " + channel + " :End of /NAMES list\r\n";
    return msg;
}

// Error replies
std::string Replies::ERR_NOSUCHNICK(const std::string& serverName, const std::string& nick, 
                                   const std::string& target)
{
    std::string msg = ":" + serverName + " 401 " + nick;
    msg += " " + target + " :No such nick/channel\r\n";
    return msg;
}

std::string Replies::ERR_NOSUCHCHANNEL(const std::string& serverName, const std::string& nick, 
                                      const std::string& channel)
{
    std::string msg = ":" + serverName + " 403 " + nick;
    msg += " " + channel + " :No such channel\r\n";
    return msg;
}

std::string Replies::ERR_CANNOTSENDTOCHAN(const std::string& serverName, const std::string& nick, 
                                         const std::string& channel)
{
    std::string msg = ":" + serverName + " 404 " + nick;
    msg += " " + channel + " :Cannot send to channel\r\n";
    return msg;
}

std::string Replies::ERR_NOTONCHANNEL(const std::string& serverName, const std::string& nick, 
                                     const std::string& channel)
{
    std::string msg = ":" + serverName + " 442 " + nick;
    msg += " " + channel + " :You're not on that channel\r\n";
    return msg;
}

std::string Replies::ERR_USERNOTINCHANNEL(const std::string& serverName, const std::string& nick, 
                                         const std::string& target, const std::string& channel)
{
    std::string msg = ":" + serverName + " 441 " + nick;
    msg += " " + target + " " + channel + " :They aren't on that channel\r\n";
    return msg;
}

std::string Replies::ERR_NEEDMOREPARAMS(const std::string& serverName, const std::string& nick, 
                                       const std::string& command)
{
    std::string msg = ":" + serverName + " 461 " + nick;
    msg += " " + command + " :Not enough parameters\r\n";
    return msg;
}

std::string Replies::ERR_ALREADYREGISTERED(const std::string& serverName, const std::string& nick)
{
    std::string msg = ":" + serverName + " 462 " + nick;
    msg += " :You may not reregister\r\n";
    return msg;
}

std::string Replies::ERR_PASSWDMISMATCH(const std::string& serverName, const std::string& nick)
{
    std::string msg = ":" + serverName + " 464 " + nick;
    msg += " :Password incorrect\r\n";
    return msg;
}

std::string Replies::ERR_NONICKNAMEGIVEN(const std::string& serverName, const std::string& nick)
{
    std::string msg = ":" + serverName + " 431 " + nick;
    msg += " :No nickname given\r\n";
    return msg;
}

std::string Replies::ERR_ERRONEUSNICKNAME(const std::string& serverName, const std::string& nick, 
                                         const std::string& badnick)
{
    std::string msg = ":" + serverName + " 432 " + nick;
    msg += " " + badnick + " :Erroneous nickname\r\n";
    return msg;
}

std::string Replies::ERR_NICKNAMEINUSE(const std::string& serverName, const std::string& nick, 
                                      const std::string& badnick)
{
    std::string msg = ":" + serverName + " 433 " + nick;
    msg += " " + badnick + " :Nickname is already in use\r\n";
    return msg;
}

std::string Replies::ERR_USERONCHANNEL(const std::string& serverName, const std::string& nick, 
                                      const std::string& target, const std::string& channel)
{
    std::string msg = ":" + serverName + " 443 " + nick;
    msg += " " + target + " " + channel + " :is already on channel\r\n";
    return msg;
}

std::string Replies::ERR_NOTREGISTERED(const std::string& serverName, const std::string& nick)
{
    std::string msg = ":" + serverName + " 451 " + nick;
    msg += " :You have not registered\r\n";
    return msg;
}

std::string Replies::ERR_CHANOPRIVSNEEDED(const std::string& serverName, const std::string& nick, 
                                         const std::string& channel)
{
    std::string msg = ":" + serverName + " 482 " + nick;
    msg += " " + channel + " :You're not channel operator\r\n";
    return msg;
}

std::string Replies::ERR_UNKNOWNMODE(const std::string& serverName, const std::string& nick, 
                                    char mode)
{
    std::string msg = ":" + serverName + " 472 " + nick;
    msg += " " + std::string(1, mode) + " :is unknown mode char to me\r\n";
    return msg;
}

std::string Replies::ERR_INVITEONLYCHAN(const std::string& serverName, const std::string& nick, 
                                       const std::string& channel)
{
    std::string msg = ":" + serverName + " 473 " + nick;
    msg += " " + channel + " :Cannot join channel (+i)\r\n";
    return msg;
}

std::string Replies::ERR_BADCHANNELKEY(const std::string& serverName, const std::string& nick, 
                                      const std::string& channel)
{
    std::string msg = ":" + serverName + " 475 " + nick;
    msg += " " + channel + " :Cannot join channel (+k)\r\n";
    return msg;
}

std::string Replies::ERR_CHANNELISFULL(const std::string& serverName, const std::string& nick, 
                                      const std::string& channel)
{
    std::string msg = ":" + serverName + " 471 " + nick;
    msg += " " + channel + " :Cannot join channel (+l)\r\n";
    return msg;
}

std::string Replies::ERR_BADCHANMASK(const std::string& serverName, const std::string& nick, 
                                    const std::string& channel)
{
    std::string msg = ":" + serverName + " 476 " + nick;
    msg += " " + channel + " :Bad Channel Mask\r\n";
    return msg;
}

std::string Replies::ERR_NORECIPIENT(const std::string& serverName, const std::string& nick, 
                                    const std::string& command)
{
    std::string msg = ":" + serverName + " 411 " + nick;
    msg += " :No recipient given (" + command + ")\r\n";
    return msg;
}

std::string Replies::ERR_NOTEXTTOSEND(const std::string& serverName, const std::string& nick)
{
    std::string msg = ":" + serverName + " 412 " + nick;
    msg += " :No text to send\r\n";
    return msg;
}

std::string Replies::RPL_CHANNELMODEIS(const std::string& serverName, const std::string& nick,
    const std::string& channel, const std::string& modes)
{
    return ":" + serverName + " 324 " + nick + " " + channel + " " + modes + "\r\n";
}

std::string Replies::ERR_USERSDONTMATCH(const std::string& serverName, const std::string& nick)
{
    return ":" + serverName + " 502 " + nick + " :Cant change mode for other users\r\n";
}