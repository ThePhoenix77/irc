#include "Server.hpp"

bool Server::sendMessageToClient(std::string target, std::string message, std::map<int, Client *> &_Client, Client &sender)
{
	std::string letter;
	for (std::map<int, Client *>::iterator it = _Client.begin(); it != _Client.end(); ++it)
	{
		if (it->second->getNickname() == target)
		{
			letter = sender.getNickname() + "!" + sender.getUsername() + "@127.0.0.1 PRIVMSG " + target + " :" + message + "\r\n";
			sendReply(*it->second, letter);
			return true;
		}
	}
	return false;
}

bool sendMessagetoChannel(std::map<std::string, Channel>& channels, Client &client, std::string &target, std::string& message)
{
	std::string broadcastMessage = ":" + client.getNickname() + " " + client.getUsername() + " [SBITAR] " + target + " :" + message + "\r\n";
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (target == it->first)
		{
			std::vector<int> list = it->second.GetClient();
			for (size_t i = 0; i < list.size(); i++)
			{
				if (list[i] == client.getFd()){
					for (size_t i = 0; i < list.size(); i++)
					{
						if (list[i] != client.getFd())
							send(list[i], broadcastMessage.c_str(), broadcastMessage.length(), 0);
					}
					return true;
				}
			}
			sendError(client, ERR_CANNOTSENDTOCHAN(client.getNickname(), target));
			return true;
		}
	}
	return false;
}

void parsePrivMessage(const std::string &input, std::string &target, std::string &message)
{
	std::istringstream iss(input);
	std::string command;
	iss >> command >> target;
	if (input.find(":") != std::string::npos)
	{
		for (size_t i = 0; i < input.length(); i++)
		{
			if (input[i] == ':')
				message = input.substr(input.find(":") + 1);
		}
		while (!message.empty() && (message[message.length() - 1] == '\r' || message[message.length() - 1] == '\n'))
			message.erase(message.length() - 1, 1);
	}
	else
		iss >> message;
}

void Server::check_privMsg(const std::string &input, Client &client)
{
	std::string target, message;
	std::vector<std::string> arr;
	std::string nickname = client.getNickname().empty() ? "*" : client.getNickname();
	if (!client.isRegistered())
	{
		sendError(client, ERR_NOTREGISTERED(nickname));
		return;
	}
	parsePrivMessage(input, target, message);
	if (target.empty() || target[0] == ':')
	{
		sendError(client, ERR_NORECIPIENT(nickname));
		return;
	}
	if (message.empty())
	{
		sendError(client, ERR_NOTEXTTOSEND(nickname));
		return;
	}
	if (target[0] != '#')
	{
		if (!sendMessageToClient(target, message, clientMap, client))
			sendError(client, ERR_NOSUCHNICK(nickname, target));
		return;
	}
	else
	{
		if (!sendMessagetoChannel(_channles, client, target, message))
			sendError(client, ERR_NONSUCHCHANNEL(nickname, target));
	}
}
