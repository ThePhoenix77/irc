#include "Server.hpp"

bool sendReply(Client &client, const std::string &replyMessage)
{
	int fd = client.getFd();
	if (fd < 0)
		return false;
	if (send(fd, replyMessage.c_str(), replyMessage.length(), 0) == -1)
	{
		return false;
	}
	return true;
}

bool sendMessage(int fd, const std::string &replyMessage)
{
	if (fd < 0)
		return false;
	if (send(fd, replyMessage.c_str(), replyMessage.length(), 0) == -1)
		return false;
	return true;
}

bool sendError(Client &client, const std::string &errorMessage)
{
	int fd = client.getFd();
	if (fd < 0)
		return false;
	if (send(fd, errorMessage.c_str(), errorMessage.length(), 0) == -1)
	{
		return false;
	}
	return true;
}

void Server::check_pass(const std::string &input, Client &client, std::string &password, std::vector<pollfd> &fds)
{
	std::istringstream iss(input);
	std::string command, pass;
	iss >> command >> pass;
	std::string nickname = client.getNickname().empty() ? "*" : client.getNickname();

	(void)fds;
	if (client.isRegistered())
	{
		sendError(client, ERR_ALREADYREGISTERED(nickname));
		return;
	}
	if (pass.empty())
	{
		sendError(client, ERR_NEEDMOREPARAMS(std::string("PASS")));
		return;
	}
	if (client.isAuthenticated())
		return;
	if (pass != password)
	{
		sendError(client, ERR_PASSWDMISMATCH(nickname));
		return;
	}
	client.authenticate();
}
