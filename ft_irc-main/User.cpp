#include "Server.hpp"

void Server::check_userName(const std::string &input, Client &client)
{
	std::istringstream iss(input);
	std::vector<std::string> arr;
	std::string arg;
	std::string realname;
	std::string nickname = client.getNickname().empty() ? "*" : client.getNickname();

	while (iss >> arg)
		arr.push_back(arg);

	if (!client.isAuthenticated())
	{
		sendError(client, ERR_NOTREGISTERED(nickname));
		return;
	}
	if (client.isRegistered())
	{
		sendError(client, ERR_ALREADYREGISTERED(nickname));
		return;
	}
	if (arr.size() < 5)
	{
		sendError(client, ERR_NEEDMOREPARAMS(std::string("USER")));
		return;
	}
	if (!arr[4].empty() && arr[4][0] == ':')
	{
		for (size_t i = 4; i < arr.size(); ++i)
		{
			realname += arr[i];
			if (i != arr.size() - 1)
				realname += " ";
		}
		realname = realname.substr(1);
	}
	else
		realname = arr[4];
	while (!realname.empty() && (realname[realname.length() - 1] == '\n' || realname[realname.length() - 1] == '\r'))
		realname.erase(realname.length() - 1, 1);
	if (realname.empty())
	{
		sendError(client, ERR_NEEDMOREPARAMS(std::string("USER")));
		return;
	}
	client.setUsername(arr[1]);
	client.setRealname(realname);
	client.registerClient();

	if (client.isRegistered())
		sendWelcomeMessages(client);
}
