#include "Server.hpp"

// void parseKickCommand(const std::string &input, std::vector<std::string> &channelName, std::vector<std::string> &client, std::string& message, std::vector<std::string> &args)
// {
// 	std::istringstream iss(input);
// 	std::string holder;
// 	while (iss >> holder)
// 		args.push_back(holder);
// 	if (!args[1].empty())
// 		channelName = split(args[1], ',');
// 	if (!args[2].empty())
// 		client = split(args[2], ',');
// 	if (input.find(":") != std::string::npos)
// 	{
// 		for (size_t i = 0; i < input.length(); i++)
// 		{
// 			if (input[i] == ':')
// 				message = input.substr(input.find(":") + 1);
// 		}
// 		while (!message.empty() && (message[message.length() - 1] == '\r' || message[message.length() - 1] == '\n'))
// 			message.erase(message.length() - 1, 1);
// 	}
// 	else{
// 		if (!args[3].empty())
// 			message = args[3];
// 	}
// }

// bool validateCommand(std::map<std::string, Channel> &channels, std::vector<std::string> &channel, std::vector<std::string> &users, Client &client, std::map<int, Client *> clientMap)
// {
// 	if (channel.size() > 1 || (!channel[0].empty() && channel[0][0] != '#'))
// 	{
// 		sendError(client, ERR_BADCHANMASK(client.getNickname(), channel[0]));
// 		return false;
// 	}
// 	if (!channelExists(channel[0], channels))
// 	{
// 		sendError(client, ERR_NONSUCHCHANNEL(client.getNickname(), channel[0]));
// 		return false;
// 	}
// 	if (!channels[channel[0]].clientExist(client.getFd()))
// 	{
// 		sendError(client, ERR_NOTONCHANNEL(client.getNickname(), channel[0]));
// 		return false;
// 	}
// 	if (!channels[channel[0]].clientIsOperator(client.getFd()))
// 	{
// 		sendError(client, ERR_CHANOPRIVSNEEDED(client.getNickname(), channel[0]));
// 		return false;
// 	}
// 	for (size_t i = 0; i < users.size(); i++)
// 	{
// 		bool found = false;
// 		for (std::map<int, Client *>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)
// 		{
// 			if (it->second->getNickname() == users[i])
// 			{
// 				found = true;
// 				if (!channels[channel[0]].clientExist(it->second->getFd()))
// 				{
// 					sendError(client, ERR_USERNOTINCHANNEL(client.getNickname(), users[i], channel[0]));
// 					return false;
// 				}
// 				break;
// 			}
// 		}
// 		if (!found)
// 		{
// 			sendError(client, ERR_USERNOTINCHANNEL(client.getNickname(), users[i], channel[0]));
// 			return false;
// 		}
// 	}
// 	return true;
// }

// void Server::kickCmd(const std::string &input, Client &client){
// 	std::string message, replyMessage;
// 	std::vector<std::string> channelName, clients, args;
// 	std::string nickname = client.getNickname().empty() ? "*" : client.getNickname();
// 	if (!client.isRegistered()){
// 		sendError(client, ERR_NOTREGISTERED(nickname));
// 		return;
// 	}
// 	parseKickCommand(input, channelName, clients, message, args);
// 	if (args.size() < 3){
// 		sendError(client, ERR_NEEDMOREPARAMS(std::string("KICK")));
// 		return ;
// 	}
// 	if (!validateCommand(_channles, channelName, clients, client, clientMap))
// 		return ;
// 	Channel *holder = &_channles[channelName[0]];
// 	Client *client_holder;
// 	for (size_t i = 0; i < clients.size(); i++){
// 		for (std::map<int, Client*>::iterator it = clientMap.begin(); it != clientMap.end(); it++){
// 			if (it->second->getNickname() == clients[i])
// 			{
// 				client_holder = it->second;
// 				break;
// 			}
// 		}
// 		holder->removeClient(client_holder->getFd());
// 		replyMessage = ":" + client.getNickname() + " KICK " + channelName[0] + " " + client_holder->getNickname() + " " + (message.empty() ? ":" + client.getNickname() : message);
// 		for (size_t i = 0; i < holder->GetClient().size(); i++){
// 			sendMessage(holder->GetClient()[i], replyMessage);
// 		}
// 	}
// }

void parseKickCommand(const std::string &input, std::vector<std::string> &channelName, std::vector<std::string> &client, std::string &message, std::vector<std::string> &args)
{
	std::istringstream iss(input);
	std::string holder;
	// std::cout << "input = " << input << "\n";
	while (iss >> holder)
		args.push_back(holder);
	// for (size_t i = 0; i < args.size(); i++)
	// {
	// 	std::cout << "args[" << i << "] = <" << args[i] << ">\n";
	// }
	if (args.size() > 1 && !args[1].empty())
		channelName = split(args[1], ',');
	if (args.size() > 2 && !args[2].empty())
		client = split(args[2], ',');
	size_t pos = input.find(':');
	if (pos != std::string::npos)
	{
		message = input.substr(pos + 1);
		while (!message.empty() && (message[message.length() - 1] == '\r' || message[message.length() - 1] == '\n'))
			message.erase(message.length() - 1, 1);
	}
	if (message.empty() && args.size() > 3)
		message = args[3];
}

bool validateCommand(std::map<std::string, Channel> &channels, std::vector<std::string> &channel, std::vector<std::string> &users, Client &client, std::map<int, Client *> &clientMap)
{
	if (channel.size() > 1 || (!channel[0].empty() && channel[0][0] != '#'))
	{
		sendError(client, ERR_BADCHANMASK(client.getNickname(), channel[0]));
		return false;
	}
	if (!channelExists(channel[0], channels))
	{
		sendError(client, ERR_NONSUCHCHANNEL(client.getNickname(), channel[0]));
		return false;
	}
	if (!channels[channel[0]].clientExist(client.getFd()))
	{
		sendError(client, ERR_NOTONCHANNEL(client.getNickname(), channel[0]));
		return false;
	}
	if (!channels[channel[0]].clientIsOperator(client.getFd()))
	{
		sendError(client, ERR_CHANOPRIVSNEEDED(client.getNickname(), channel[0]));// something is not right in here;
		return false;
	}
	for (size_t i = 0; i < users.size(); i++)
	{
		bool found = false;
		for (std::map<int, Client *>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)
		{
			if (it->second->getNickname() == users[i])
			{
				found = true;
				if (!channels[channel[0]].clientExist(it->second->getFd()))
				{
					sendError(client, ERR_USERNOTINCHANNEL(client.getNickname(), users[i], channel[0]));
					return false;
				}
				break;
			}
		}
		if (!found)
		{
			sendError(client, ERR_USERNOTINCHANNEL(client.getNickname(), users[i], channel[0]));
			return false;
		}
	}
	return true;
}

void checkEmptychannel(Channel &channel, std::map<std::string, Channel> &_channels)
{
	if (channel.GetClient().size() == 0)
		_channels.erase(channel.GetChannelName());
}

void Server::kickCmd(const std::string &input, Client &client)
{
	std::string message, replyMessage;
	std::vector<std::string> channelName, clients, args;
	std::string nickname = client.getNickname().empty() ? "*" : client.getNickname();

	if (!client.isRegistered())
	{
		sendError(client, ERR_NOTREGISTERED(nickname));
		return;
	}
	parseKickCommand(input, channelName, clients, message, args);
	for (size_t i = 0; i < args.size(); i++)
	{
		std::cout << "args[" << i << "] = <" << args[i] << ">\n";
	}

	if (args.size() < 3)
	{
		sendError(client, ERR_NEEDMOREPARAMS(std::string("KICK")));
		return;
	}

	if (!validateCommand(_channles, channelName, clients, client, clientMap))
		return;

	Channel *holder = &_channles[channelName[0]];

	for (size_t i = 0; i < clients.size(); i++)
	{
		Client *client_holder = NULL;
		for (std::map<int, Client *>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)
		{
			if (it->second->getNickname() == clients[i])
			{
				client_holder = it->second;
				break;
			}
		}
		if (!client_holder)
			continue;
		holder->removeClient(client_holder->getFd());
		replyMessage = ":" + client.getNickname() + " KICK " + channelName[0] + " " + client_holder->getNickname() + " " + (message.empty() ? ":" + client.getNickname() : ":" + message) + "\r\n";
		std::vector<int> &channelClients = holder->GetClient();
		for (size_t j = 0; j < channelClients.size(); ++j)
			sendMessage(channelClients[j], replyMessage);
	}
	checkEmptychannel(*holder, _channles);
}
