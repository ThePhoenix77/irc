/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 20:22:38 by tboussad          #+#    #+#             */
/*   Updated: 2025/04/26 16:07:11 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// Constructor
Client::Client(int client_fd) : fd(client_fd), hasPassword(false), _hasNick(false), _hasUser(false), authenticated(false), registered(false)
{
	std::cout << "[SBITAR] Client connected" << std::endl;
}

Client::~Client(){
    std::cout << "[SBITAR] Client disconnected" << std::endl;
}

// Getters
int Client::getFd() const{
    return fd;
}

const std::string& Client::getNickname() const{
    return nickname;
}

const std::string& Client::getUsername() const{
    return username;
}

const std::string& Client::getRealname() const{
    return realname;
}

// Setters
void Client::setNickname(const std::string& nick){
    nickname = nick;
    _hasNick = true;
}

void Client::setUsername(const std::string& user){
    username = user;
    _hasUser = true;
}

void Client::setRealname(const std::string& real){
    realname = real;
}

// Authentication & Registration
void Client::authenticate(){
    hasPassword = true;
    authenticated = true;
}

void Client::registerClient(){
    if (_hasNick && _hasUser && authenticated)
        registered = true;
}

// Status Checks
void Client::setHasNick(bool value){
    _hasNick = value;
}

void Client::setHasUser(bool value){
    _hasUser = value;
}

void Client::setHasPassword(bool value){
    hasPassword = value;
}

bool Client::isAuthenticated() const{
    return authenticated;
}

bool Client::isRegistered() const{
    return registered;
}

bool Client::hasNick() const{
	return _hasNick;
}

bool Client::hasUser() const{
	return _hasUser;
}
