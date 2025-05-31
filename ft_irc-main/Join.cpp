#include "Server.hpp"

void GetChannelsInfo(std::vector<std::string> &_string, std::vector<std::string> &ChannelName, std::vector<std::string> &channelPass, const std::string &input)
{
	std::istringstream iss(input);
	std::string arg;
	while (iss >> arg)
		_string.push_back(arg);
	if (_string.size() > 1 && !_string[1].empty())
		ChannelName = split(_string[1], ',');
	for (size_t i = 0; i < ChannelName.size(); i++)
	{
		std::cout << "channelname = " << ChannelName[i] << "\n";
	}
	if (_string.size() > 2 && !_string[2].empty())
		channelPass = split(_string[2], ',');
	for (size_t i = 0; i < channelPass.size(); i++)
	{
		std::cout << "channelPass = " << channelPass[i] << "\n";
	}
}

bool channelExists(const std::string &channel, std::map<std::string, Channel> &channels)
{
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (channel == it->first)
			return true;
	}
	return false;
}

bool check_password(std::vector<std::string>& channelPassword, Channel& channel){
	for (size_t i = 0; i < channelPassword.size(); i++)
	{
		if (channelPassword[i] == channel.GetPassword())
			return true;
	}
	return false;
}

void BroadcastJoinMessage(Server& server, Channel& channel, Client& client){
	std::vector<int> clientsFds = channel.GetClient();
	std::string clientReply = ":[SBITAR] 353 " + client.getNickname() + " = " + channel.GetChannelName() + " : @" + client.getNickname() + " ";
	for (size_t i = 0; i < clientsFds.size(); i++)
	{
		if (client.getFd() != clientsFds[i])
		{
			if (!server.GetClientNick(clientsFds[i]).empty())
				clientReply += "@" + server.GetClientNick(clientsFds[i]) + " ";
		}
	}
	clientReply += "\r\n:[SBITAR] 366 " + client.getNickname() + " " + channel.GetChannelName() + " :End of NAMES list\r\n";
	sendReply(client, JOIN_REPLY(client.getNickname(), client.getUsername(), std::string("[SBITAR]"), channel.GetChannelName()));
	sendReply(client, clientReply);
	clientReply = JOIN_REPLY(client.getNickname(), client.getUsername(), std::string("[SBITAR]"), channel.GetChannelName());
	for (size_t i = 0; i < clientsFds.size(); i++)
	{
		if (client.getFd() != clientsFds[i])
		{
			if (send(clientsFds[i], clientReply.c_str(), clientReply.length(), 0) == 0)
				return ;
		}
	}

}

void channelHasPassword(Server &server, const std::string &pass, Channel &channel, Client &client)
{
	if (pass.empty() || pass != channel.GetPassword())
	{
		sendError(client, ERR_BADCHANNELKEY(client.getNickname(), channel.GetChannelName()));
		return ;
	}
	if (channel.AddClient(client.getFd()))
		BroadcastJoinMessage(server, channel, client);
	return ;
}

void Server::check_channel(const std::string &input, Client &client)
{
	int index = 0;
	std::vector<std::string> arr;
	std::vector<std::string> ChannelName, ChannelPass;
	std::string nickname = client.getNickname().empty() ? "*" : client.getNickname();
	if (!client.isRegistered()){
		sendError(client, ERR_NOTREGISTERED(nickname));
		return;
	}
	GetChannelsInfo(arr, ChannelName, ChannelName, input);
	if (arr.size() == 1){
		sendError(client, ERR_NEEDMOREPARAMS(std::string("JOIN")));
		return;
	}
	for (size_t i = 0; i < ChannelName.size(); i++){
		std::cout << "channle name is [" << ChannelName[i] << "]\n";
		if (ChannelName[i][0] != '#' || ChannelName[i].size() == 1){
			sendError(client, ERR_NONSUCHCHANNEL(nickname, ChannelName[i]));
			continue;
		}
		if (channelExists(ChannelName[i], _channles)) // if channel exists , then i need to add the client to it if not already in it
		{
			//added code snippet for MODE command to add the user linit check before adding it to an existing channel
        	if (_channles[ChannelName[i]].getUserLimit() > 0 && _channles[ChannelName[i]].GetClient().size() >= static_cast<size_t>(_channles[ChannelName[i]].getUserLimit())) {
            	sendError(client, ERR_CHANNELISFULL(client.getNickname(), _channles[ChannelName[i]].GetChannelName()));
            	continue;
        	}
			if (_channles[ChannelName[i]].GetHasPassword()) // it could segfault when using a password
			{
				channelHasPassword(*this, ChannelPass[index++], _channles[ChannelName[i]], client);
				continue;
			}
			else // if channle eists but not pass
			{
				if (_channles[ChannelName[i]].AddClient(client.getFd()))
					BroadcastJoinMessage(*this, _channles[ChannelName[i]], client);
				continue;
			}
		}
		else // if channel does not exists
		{
			Channel new_channel(ChannelName[i]);
			new_channel.AddClient(client.getFd());
			new_channel.AddModerator(client.getFd());
			_channles[ChannelName[i]] = new_channel;
			BroadcastJoinMessage(*this, _channles[ChannelName[i]], client);
		}
	}
}
