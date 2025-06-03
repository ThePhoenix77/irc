#include "Server.hpp"

void Server::handleInvite(const std::string& input, Client& client){
    std::stringstream iss(input);
    std::string command, target_nickname, channel;
    iss >> command >> target_nickname >> channel;

    std::string nickname = client.getNickname();
    if (!client.isRegistered() /* || nickname.empty()*/) {
        sendError(client, ERR_NOTREGISTERED2("INVITE"));
        return;
    }
    if(target_nickname.empty() || channel.empty()) {
        sendError(client, ERR_NEEDMOREPARAMS("INVITE"));
        return;
    }
    // if (channel[0] != '#') {
    //     sendError(client, ERR_NOSUCHCHANNEL(channel));
    //     return;
    // }
    if(_channles.find(channel) == _channles.end()) {
        sendError(client, ERR_NOSUCHCHANNEL(channel));
        return;
    }
    Channel& chan = _channles[channel];
    if(!chan.clientExist(client.getFd())) {
        sendError(client, ERR_NOTONCHANNEL(nickname, channel));
        return;
    }
    if(!chan.isOperator(client.getFd())) {
        sendError(client, ERR_CHANOPRIVSNEEDED(nickname, channel));
        return;
    }
    Client* target_client = NULL;
    for (std::map<int, Client *>::iterator it = clientMap.begin(); it != clientMap.end(); ++it){
        if (it->second->getNickname() == target_nickname){
            target_client = it->second;
            break;
        }
    }
    if (!target_client){
        sendError(client, ERR_NOSUCHNICK(nickname, target_nickname));
        return;
    }
    std::string inviteMessage = ":" + client.getNickname() + " INVITE " + target_nickname + " :" + channel + "\r\n";
    sendReply(*target_client, inviteMessage);
    sendReply(client, ":" + std::string("server") + " 341 " + client.getNickname() + " " + target_nickname + " " + channel + "\r\n");
}