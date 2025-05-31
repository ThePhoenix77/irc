/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 20:22:40 by tboussad          #+#    #+#             */
/*   Updated: 2025/05/17 15:42:26 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <memory>
#include "Server.hpp"

class Client{
    private:
        int fd;
		// int passwrodRetries;
        std::string nickname;
        std::string username;
        std::string realname;
        std::string buffer;

        bool hasPassword;
        bool _hasNick;
        bool _hasUser;

        bool authenticated;
        bool registered;

    public:
        Client(int client_fd);
        ~Client();

        // Status
        bool isAuthenticated() const;
        bool isRegistered() const;

        // Getters
        int getFd() const;
        const std::string& getNickname() const;
        const std::string& getUsername() const;
        const std::string& getRealname() const;
		bool hasNick() const;
		bool hasUser() const;
		// std::string& getBuffer();

        // Setters
        void setNickname(const std::string& nickname);
        void setUsername(const std::string& username);
        void setRealname(const std::string& realname);
        void authenticate();
        void registerClient();
		// void incrementPasswordRetries();
		// void resetPasswordRetries() { passwrodRetries = 0; }

        void setHasPassword(bool value);
        void setHasNick(bool value);
        void setHasUser(bool value);

        // void tryRegister();
        // void appendBuffer(const std::string& data);
};



#endif
