/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:56:10 by tboussad          #+#    #+#             */
/*   Updated: 2025/05/23 17:50:05 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, const std::string &password) : serverSocket(-1), serverPort(port), serverPassword(password)
{
	initializeSocket();
}

Server::~Server()
{
	if (serverSocket >= 0)
		close(serverSocket);

	// Clean up all client objects
	for (std::map<int, Client *>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)
	{
		delete it->second;
	}
}

void Server::setSocketNonBlocking(int socketFd)
{
	fcntl(socketFd, F_SETFL, O_NONBLOCK);
}

void Server::initializeSocket()
{
	int socketOption = 1;

	// Create socket for IPv4 and TCPSBITAR] Client disconnected
	// [SBITAR] Client connected
		// [IRC] New client connected(fd = 4)
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
	{
		std::cerr << "Error: Failed to create socket" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Set socket option to allow address reuse
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &socketOption, sizeof(socketOption));

	// Set to non-blocking mode
	setSocketNonBlocking(serverSocket);

	// Server address configuration
	struct sockaddr_in serverAddress;
	std::memset(&serverAddress, 0, sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverPort);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Bind socket to address
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cerr << "Error: Failed to bind to port " << serverPort << std::endl;
		exit(EXIT_FAILURE);
	}

	// Start listening for connections
	if (listen(serverSocket, 10) < 0)
	{
		std::cerr << "Error: Failed to listen on socket" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Add server socket to poll list
	struct pollfd pollSocket;
	pollSocket.fd = serverSocket;
	pollSocket.events = POLLIN; // SHOULD LISTEN ALSO ON POLLOUT TO CHECK IF THE SOCKET IS READY TO WRITE AS WELL
	pollSocket.revents = 0;
	pollFds.push_back(pollSocket);

	// Generate server token from /dev/urandom for additional security features
	generateServerToken();

	std::cout << "[IRC] Server listening on port " << serverPort << std::endl;
}

void Server::generateServerToken()
{
	std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
	if (!urandom)
	{
		std::cerr << "Warning: Failed to open /dev/urandom, using fallback for server token" << std::endl;
		// C++98 compatible way to convert time to string
		std::stringstream ss;
		ss << "default_token_" << time(NULL);
		serverToken = ss.str();
		return;
	}

	unsigned char buffer[16];
	urandom.read(reinterpret_cast<char *>(buffer), sizeof(buffer));

	std::stringstream hexToken;
	for (size_t i = 0; i < sizeof(buffer); ++i)
	{
		hexToken << std::hex << static_cast<int>(buffer[i]);
	}

	serverToken = hexToken.str();
	urandom.close();
}

void Server::run()
{
	while (true)
	{
		// Poll for events with no timeout (wait indefinitely)
		int eventCount = poll(&pollFds[0], pollFds.size(), -1);
		if (eventCount < 0)
		{
			std::cerr << "Error: Poll operation failed" << std::endl;
			exit(EXIT_FAILURE);
		}

		for (size_t i = 0; i < pollFds.size(); ++i)
		{
			if (pollFds[i].revents & POLLIN)
			{
				if (pollFds[i].fd == serverSocket)
					acceptNewConnection();
				else
				{
					int clientSocket = pollFds[i].fd;
					Client *client = clientMap[clientSocket];

					// Handle client data
					// EACH CLIENT MUST HAVE ITS OWN BUFFER
					// IGNORE CTRL + D IN THE BUFFER
					// ADD A WHILE LOOP OVER THE BUFFER SPLIT BY \R\N AND \N THEN SEND EACH PART TO BE EXECUTED SEPARETLY, THE REST MUST BE KEPT IN THE BUFFER .
					char buffer[1024];
					int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

					if (bytesReceived <= 0)
					{
						// Client disconnected or error
						close(clientSocket);
						std::cout << "[IRC] Client disconnected" << std::endl;
						pollFds.erase(pollFds.begin() + i);

						delete client;
						clientMap.erase(clientSocket);
						--i; // Adjust index after erasing
					}
					else
					{
						buffer[bytesReceived] = '\0';
						std::string input(buffer);
						parseCommand(input, *client);
					}
				}
			}
		}
	}
}

void Server::acceptNewConnection()
{
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);

	int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
	if (clientSocket < 0)
	{
		std::cerr << "Error: Failed to accept connection" << std::endl;
		return;
	}

	// Create a new client object and add to client map
	clientMap.insert(std::make_pair(clientSocket, new Client(clientSocket)));

	// Set client socket to non-blocking
	setSocketNonBlocking(clientSocket);

	// Add client socket to poll list
	struct pollfd clientPollFd;
	clientPollFd.fd = clientSocket;
	clientPollFd.events = POLLIN;
	clientPollFd.revents = 0;
	pollFds.push_back(clientPollFd);

	std::cout << "[IRC] New client connected (fd = " << clientSocket << ")" << std::endl;
}

std::string Server::getFormattedCurrTime()
{
	char timeBuf[20];
	time_t now = time(NULL);
	strftime(timeBuf, sizeof(timeBuf), "%d-%m-%Y %H:%M:%S", localtime(&now));
	return std::string(timeBuf);
}

void Server::parseCommand(const std::string &input, Client &client)
{
	std::istringstream iss(input);
	std::string command;
	iss >> command;

	command = modifie_string(command, 0);
	if (command == "PASS")
		check_pass(input, client, serverPassword, pollFds);
	else if (command == "NICK")
		check_nick(input, client, clientMap);
	else if (command == "USER")
		check_userName(input, client);
	else if (command == "PRIVMSG")
		check_privMsg(input, client);
	else if (command == "QUIT")
		remove_client(client, input);
	else if (command == "JOIN")
		check_channel(input, client);
	else if (command == "KICK")
		kickCmd(input, client);
	else if (command == "TOPIC")
    	handleTopic(input, client);
	else if (command == "MODE")
		handleMode(input, client);
	else
		sendReply(client, "UNKNOWN COMMAND\r\n");
}

std::string Server::GetClientNick(int fd)
{
	for (std::map<int, Client *>::iterator it = clientMap.begin(); it != clientMap.end(); it++)
	{
		if (it->first == fd)
			return it->second->getNickname();
	}
	return std::string("");
}

Channel *Server::getChannel(std::string name)
{
	for (std::map<std::string, Channel>::iterator it = _channles.begin(); it != _channles.end(); it++)
	{
		if (it->first == name)
			return &it->second;
	}
	return 0;
}
