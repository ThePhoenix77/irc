#include "Server.hpp"


void Server::remove_client(Client &client, const std::string &input)
{
	std::string quitMessage;
	std::map<int, Client *>::iterator holder;
	std::string nickname = client.getNickname().empty() ? "*" : client.getNickname();

	if (!client.isRegistered())
	{
		sendError(client, ERR_NOTREGISTERED(nickname));
		return;
	}
	if (input.length() > 4)
		quitMessage = input.substr(5);
	std::string message = ":" + client.getNickname() + " QUIT " + (quitMessage.empty() ? " Client Quit " : quitMessage) + "\n";
	sendReply(client, "Goodbye! You are now disconnected.");
	for (std::map<int, Client *>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)
	{
		if (it->second->getNickname() != client.getNickname())
			sendReply(*it->second, message);
		else
			holder = it;
	}
	close(client.getFd());
	for (std::vector<pollfd>::iterator fd_it = pollFds.begin(); fd_it != pollFds.end(); ++fd_it)
	{
		if (fd_it->fd == client.getFd())
		{
			pollFds.erase(fd_it);
			break;
		}
	}
	clientMap.erase(holder);
}
