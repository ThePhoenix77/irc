#include "Server.hpp"

void Server::handleMode(const std::string& input, Client& client) {
    std::istringstream iss(input);
    std::string cmd, channelName, modes, param;
    iss >> cmd >> channelName >> modes;

    if (channelName.empty() || channelName[0] != '#') {
        sendError(client, ERR_NEEDMOREPARAMS("MODE"));
        return;
    }
    if (_channles.find(channelName) == _channles.end()) {
        sendError(client, ERR_NONSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }
    Channel& channel = _channles[channelName];

    // If no mode string, show current modes
    if (modes.empty()) {
        std::string modeStr = "+";
        if (channel.isInviteOnly()) modeStr += "i";
        if (channel.isTopicOpOnly()) modeStr += "t";
        if (!channel.getPassword().empty()) modeStr += "k";
        if (channel.getUserLimit() > 0) modeStr += "l";
        sendReply(client, ":" + std::string("server") + " 324 " + client.getNickname() + " " + channelName + " " + modeStr + "\r\n");
        return;
    }

    // Only operators can change modes
    if (!channel.isOperator(client.getFd())) {
        sendError(client, ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
        return;
    }

    bool adding = true;
    std::vector<std::string> params;
    while (iss >> param) params.push_back(param);
    size_t paramIndex = 0;

    for (size_t i = 0; i < modes.size(); ++i) {
        char mode = modes[i];
        if (mode == '+') adding = true;
        else if (mode == '-') adding = false;
        else if (mode == 'i') channel.setInviteOnly(adding);
        else if (mode == 't') channel.setTopicOpOnly(adding);
        else if (mode == 'k') {
            if (adding) {
                if (paramIndex < params.size()) {
                    channel.setPassword(params[paramIndex++]);
                }
            } else {
                channel.setPassword("");
            }
        }
        else if (mode == 'l') {
            if (adding) {
                if (paramIndex < params.size()) {
                    int limit = atoi(params[paramIndex++].c_str());
                    channel.setUserLimit(limit);
                }
            } else {
                channel.setUserLimit(0);
            }
        }
        // +o/-o: operator management (requires nickname to fd mapping)
        // else if (mode == 'o') { ... }
    }
    // Optionally, send a MODE reply to the channel
}