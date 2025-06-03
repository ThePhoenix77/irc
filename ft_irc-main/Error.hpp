#ifndef ERROR_HPP
#define ERROR_HPP



#define ENDING 	"\r\n"
#define PREFIX ":[SBITAR]"


//error codes

#define ERR_NEEDMOREPARAMS(command) (std::string(PREFIX) + " 461 " + command + " :Not enough parameters" + ENDING)
//#define ERR_NEEDMOREPARAMS(command) PREFIX " 461 " + command + " :Not enough parameters" + ENDING
#define ERR_ALREADYREGISTERED(nickname) PREFIX " 462 " + nickname + " :You may not reregister" + ENDING
#define ERR_NOTREGISTERED(nickname) PREFIX " 451 " + nickname + " :You have not registered" + ENDING
#define ERR_PASSWDMISMATCH(nickname) PREFIX " 464 " + nickname + " :Password incorrect" + ENDING
#define ERR_NONICKNAMEGIVEN(nickname) PREFIX " 431 " + nickname + " :No nickname given" + ENDING
#define ERR_ERRONEUSNICKNAME(nickname, nick) PREFIX " 432 " + nickname + " " + nick + " :Erroneus nickname" + ENDING
#define ERR_NICKNAMEINUSE(nickname, nick) PREFIX " 433 " + nickname + " " + nick + " :Nickname is already in use" + ENDING
#define ERR_PASSEDTHREETRIES(nickname) PREFIX " NOTICE " + nickname + ": Connection closed after 3 failed password attempts" + ENDING

//PRIVMSG
#define ERR_NORECIPIENT(nickname) PREFIX " 411 " + nickname + " :No recipient given (PRIVMSG)" + ENDING
#define ERR_NOTEXTTOSEND(nickname) PREFIX " 412 " + nickname + " :No text to send" + ENDING
#define ERR_NOSUCHNICK(nickname, target) PREFIX " 401 " + nickname + " " + target + " :No such nick" + ENDING
#define ERR_CANNOTSENDTOCHAN(nickname, channel) PREFIX " 404 " + nickname + " " + channel + " :Cannot send to channel" + ENDING
#define ERR_NONSUCHCHANNEL(nickname, channel) PREFIX " 403 " + nickname + " " + channel + " :No such channel" + ENDING
#define JOIN_REPLY(nickname, username, hostname, channel) ":" + nickname + "!" + username + "@" + hostname + " JOIN " + channel + ENDING
#define CREATE_NEW_CHANNEL()
#define ERR_BADCHANNELKEY(nickname, channel) PREFIX " 475 " + nickname + " " + channel + " :Cannot join channel (+k)" + ENDING
#define ERR_CHANOPRIVSNEEDED(nickname, channel) PREFIX " 482 " + nickname + " " + channel + " :You're not channel operator" + ENDING
#define ERR_USERNOTINCHANNEL(nickname, user, channel) PREFIX " 441 " + nickname + " " + user + " " + channel + " :They aren't on that channel" + ENDING
#define ERR_NOTONCHANNEL(nickname, channel) PREFIX " 442 " + nickname + " " + channel + " :You're not on that channel" + ENDING
#define ERR_BADCHANMASK(nickname, channel) PREFIX " 476 " + nickname + " " + channel + " :Bad Channel Mask" + ENDING

//topic
#define RPL_TOPIC(nick, channel, topic) (":" + std::string("server") + " 332 " + nick + " " + channel + " :" + topic + "\r\n")+ ENDING
#define RPL_NOTOPIC(nick, channel) (":" + std::string("server") + " 331 " + nick + " " + channel + " :No topic is set\r\n")+ ENDING
#define ERR_CHANNELISFULL(nickname, channel) PREFIX " 471 " + nickname + " " + channel + " :Cannot join channel (+l)" + ENDING

//invite
#define ERR_NOSUCHCHANNEL(channel) (":" + std::string("server") + " 403 " + channel + " :No such channel\r\n") + ENDING
#define ERR_NOTREGISTERED2(command) (":" + std::string("server") + " 451 " + command + " :You have not registered\r\n") + ENDING

#define RPL_INVITING(nickname, target, channel) PREFIX " 341 " + nickname + " " + target + " " + channel + ENDING
#define ERR_CHANOPRIVSNEEDED(nickname, channel) PREFIX " 482 " + nickname + " " + channel + " :You're not channel operator" + ENDING

//#define ERR_CHANOPRIVSNEEDED(nick, channel) (":" + std::string("server") + " 482 " + nick + " " + channel + " :You're not channel operator\r\n")+ ENDING

#endif
