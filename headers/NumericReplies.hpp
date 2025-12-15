#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>
#include <sstream>

class Replies 
{
    public:
        // welcome messages 001 004
        static std::string RPL_WELCOME(const std::string& serverName, const std::string& nick, 
                                    const std::string& user, const std::string& host);
        static std::string RPL_YOURHOST(const std::string& serverName, const std::string& nick);
        static std::string RPL_CREATED(const std::string& serverName, const std::string& nick);
        static std::string RPL_MYINFO(const std::string& serverName, const std::string& nick);

        // channel replies 353 366
        static std::string RPL_NAMREPLY(const std::string& serverName, const std::string& nick, 
                                    const std::string& channel, const std::string& names);
        static std::string RPL_ENDOFNAMES(const std::string& serverName, const std::string& nick, 
                                        const std::string& channel);

        // error replies 400 499
        static std::string ERR_NOSUCHNICK(const std::string& serverName, const std::string& nick, 
                                        const std::string& target);
        static std::string ERR_NOSUCHCHANNEL(const std::string& serverName, const std::string& nick, 
                                            const std::string& channel);
        static std::string ERR_CANNOTSENDTOCHAN(const std::string& serverName, const std::string& nick, 
                                            const std::string& channel);
        static std::string ERR_NOTONCHANNEL(const std::string& serverName, const std::string& nick, 
                                        const std::string& channel);
        static std::string ERR_USERNOTINCHANNEL(const std::string& serverName, const std::string& nick, 
                                            const std::string& target, const std::string& channel);
        static std::string ERR_NEEDMOREPARAMS(const std::string& serverName, const std::string& nick, 
                                            const std::string& command);
        static std::string ERR_ALREADYREGISTERED(const std::string& serverName, const std::string& nick);
        static std::string ERR_PASSWDMISMATCH(const std::string& serverName, const std::string& nick);
        static std::string ERR_NONICKNAMEGIVEN(const std::string& serverName, const std::string& nick);
        static std::string ERR_ERRONEUSNICKNAME(const std::string& serverName, const std::string& nick, 
                                            const std::string& badnick);
        static std::string ERR_NICKNAMEINUSE(const std::string& serverName, const std::string& nick, 
                                            const std::string& badnick);
        static std::string ERR_USERONCHANNEL(const std::string& serverName, const std::string& nick, 
                                            const std::string& target, const std::string& channel);
        static std::string ERR_NOTREGISTERED(const std::string& serverName, const std::string& nick);
        static std::string ERR_CHANOPRIVSNEEDED(const std::string& serverName, const std::string& nick, 
                                            const std::string& channel);
        static std::string ERR_UNKNOWNMODE(const std::string& serverName, const std::string& nick, 
                                        char mode);
        static std::string ERR_INVITEONLYCHAN(const std::string& serverName, const std::string& nick, 
                                            const std::string& channel);
        static std::string ERR_BADCHANNELKEY(const std::string& serverName, const std::string& nick, 
                                            const std::string& channel);
        static std::string ERR_CHANNELISFULL(const std::string& serverName, const std::string& nick, 
                                            const std::string& channel);
        static std::string ERR_BADCHANMASK(const std::string& serverName, const std::string& nick, 
                                        const std::string& channel);
        static std::string ERR_NORECIPIENT(const std::string& serverName, const std::string& nick, 
                                        const std::string& command);
        static std::string ERR_NOTEXTTOSEND(const std::string& serverName, const std::string& nick);


        static std::string RPL_CHANNELMODEIS(const std::string& serverName, const std::string& nick, const std::string& channel, const std::string& modes);
        static std::string ERR_USERSDONTMATCH(const std::string& serverName, const std::string& nick);
};

#endif