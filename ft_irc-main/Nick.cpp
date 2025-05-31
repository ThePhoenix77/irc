#include "Server.hpp"

void Server::sendWelcomeMessages(Client &client)
{
	std::string nickname = client.getNickname().empty() ? "*" : client.getNickname();
	std::string hostname = "IRC_SERVER";

	std::string clientHost = "127.0.0.1";

	std::string datetime = getFormattedCurrTime();
	std::vector<std::string> welcome_msg;
	welcome_msg.push_back("★━━━━━━━━━━━━━━━━━━ Welcome to IRC ━━━━━━━━━━━━━━━━━★");
	welcome_msg.push_back("【Connection Details】");
	welcome_msg.push_back("✦ Nickname: " + nickname);
	welcome_msg.push_back("✦ Username: " + client.getUsername());
	welcome_msg.push_back("✦ Hostname: " + clientHost);
	welcome_msg.push_back("【Server Information】");
	welcome_msg.push_back("⚡ Server: " + hostname);
	welcome_msg.push_back("⚡ Connected: " + datetime);

	for (size_t i = 0; i < welcome_msg.size(); ++i)
	{
		std::string message = ":" + hostname + " 001 " + nickname + " :" + welcome_msg[i] + "\r\n";
		sendReply(client, message);
	}
}   

void Server::check_nick(const std::string &input, Client &client, std::map<int, Client *> &clients)
{
	std::istringstream iss(input);
	std::string command, nick;
	std::string nickname = client.getNickname().empty() ? "*" : client.getNickname();
	iss >> command >> nick;

	if (!client.isAuthenticated())
	{
		sendError(client, ERR_NOTREGISTERED(nickname));
		return;
	}
	if (nick.empty())
	{
		sendError(client, ERR_ERRONEUSNICKNAME(nickname, nick));
		return;
	}
	if (nick[0] == '#' || nick[0] == ':' || nick[0] == ' ' || std::isdigit(nick[0]))
	{
		sendError(client, ERR_ERRONEUSNICKNAME(nickname, nick));
		return;
	}
	for (size_t i = 0; i < nick.length(); i++)
	{
		if (nick[i] && !std::isalnum(nick[i]) && nick[i] != '[' && nick[i] != ']' && nick[i] != '{' && nick[i] != '}' && nick[i] != '|' && nick[i] != '\\' && nick[i] != '-' && nick[i] != '_' && nick[i] != '`' && nick[i] != '^')
		{
			sendError(client, ERR_ERRONEUSNICKNAME(nickname, nick));
			return;
		}
	}
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->first != client.getFd() && it->second->getNickname() == nick)
		{
			sendError(client, ERR_NICKNAMEINUSE(nickname, nick));
			return;
		}
	}
	if (client.isRegistered())
	{
		std::string message = ":" + client.getNickname() + "!" + client.getUsername() + "@" + "IRC_SERVER" + " NICK " + nick + "\r\n";
		for (std::map<int, Client*>::iterator it = clientMap.begin() ; it != clientMap.end(); it++)
		{
			if (send(it->second->getFd(), message.c_str(), message.length(), 0) == -1)
				return;
		}

		client.setNickname(nick);
		return;
	}
	client.setNickname(nick);
	client.registerClient();
	if (client.isRegistered())
		sendWelcomeMessages(client);
}
