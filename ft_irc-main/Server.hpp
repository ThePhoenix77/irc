/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:30:32 by tboussad          #+#    #+#             */
/*   Updated: 2025/05/21 11:49:16 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <poll.h>
#include <cstdlib>
#include <cerrno>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "Client.hpp"
#include "Error.hpp"
#include "channel.hpp"

class Client;
class Channel;

class Server{
    private:
        int serverSocket;
        int serverPort;
        std::string serverPassword;
        std::string serverToken;
        std::vector<pollfd> pollFds;

        void initializeSocket();
        void setSocketNonBlocking(int socketFd);
        void acceptNewConnection();
        void generateServerToken();
		std::map<std::string, Channel> _channles;

		std::map<int, Client*> clientMap;

		public:

        void parseCommand(const std::string& input, Client& client);
		Server(int port, const std::string &password);
		~Server();

        void run();




		//anas - part :


		//user authentication commands
		void check_userName(const std::string &input, Client &client);
		void check_pass(const std::string &input, Client &client, std::string &password, std::vector<pollfd> &fds);
		void check_nick(const std::string &input, Client &client, std::map<int, Client *> &clients);
		void check_channel(const std::string& input, Client &client);
		void kickCmd(const std::string &input, Client &client);

		//get current time function
		std::string getFormattedCurrTime();


		//for sending replies to the clients
		void sendWelcomeMessages(Client &client);


		//server commands and there tools
		void check_privMsg(const std::string& input, Client& client);
		bool sendMessageToClient(std::string target, std::string message, std::map<int, Client *> &_Client, Client &sender);


		//QUIT command
		void remove_client(Client& client, const std::string& input);
		std::string GetClientNick(int fd);
		Channel* getChannel(std::string name);
		// void checkUnkown(const std::string &input);

		//TOPIC command
		void handleTopic(const std::string& input, Client& client);
		//MODE command
		void handleMode(const std::string& input, Client& client);
		//INVITE command
		void handleInvite(const std::string& input, Client& client);
};

bool sendReply(Client &client, const std::string &replyMessage);
bool sendError(Client& client, const std::string &errorMessage);
std::string& modifie_string(std::string& str, int x);
std::vector<std::string> split(const std::string &input, char delimiter);
bool channelExists(const std::string &channel, std::map<std::string, Channel> &channels);
bool sendMessage(int fd, const std::string &replyMessage);

#endif
