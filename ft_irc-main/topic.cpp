#include "Server.hpp"

void Server::handleTopic(const std::string& input, Client& client) {
    std::istringstream iss(input);
    std::string cmd, channelName, topic;
    iss >> cmd >> channelName;
    std::getline(iss, topic);
    if (!topic.empty() && topic[0] == ' ')
        topic = topic.substr(1);

    if (channelName.empty() || channelName[0] != '#') {
        sendError(client, ERR_NEEDMOREPARAMS("TOPIC"));
        return;
    }
    if (_channles.find(channelName) == _channles.end()) {
        sendError(client, ERR_NONSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }
    Channel& channel = _channles[channelName];

    // If no topic provided, just show the topic
    if (topic.empty()) {
        if (channel.getTopic().empty())
            sendReply(client, RPL_NOTOPIC(client.getNickname(), channelName));
        else
            sendReply(client, RPL_TOPIC(client.getNickname(), channelName, channel.getTopic()));
        return;
    }

    // Only operators can set the topic
    if (!channel.clientIsOperator(client.getFd())) {
        sendError(client, ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
        return;
    }

    // Set the topic
    channel.setTopic(topic);
    sendReply(client, RPL_TOPIC(client.getNickname(), channelName, topic));
}