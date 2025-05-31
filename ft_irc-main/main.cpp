/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:16:05 by tboussad          #+#    #+#             */
/*   Updated: 2025/04/23 18:01:45 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*  * Creates a TCP server socket               Done

    * Binds to a port (default: 6667)           Done

    * Listens for incoming connections          Done

    * Accepts new clients                       Done

    * Uses poll to handle multiple clients      Done

    * Handling messages from clients            Done
*/


#include <iostream>
#include <string>
#include <cstdlib>
#include "Server.hpp"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return EXIT_FAILURE;
    }
    
    int port;
    try {
        port = std::atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            std::cerr << "Error: Port must be between 1 and 65535" << std::endl;
            return EXIT_FAILURE;
        }
    } catch (...) {
        std::cerr << "Error: Invalid port number" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::string password = argv[2];
    if (password.empty()) {
        std::cerr << "Error: Password cannot be empty" << std::endl;
        return EXIT_FAILURE;
    }
    
    try {
        // Create server on the heap instead of stack to avoid stack overflow
        Server* server = new Server(port, password);
        
        // Run server
        server->run();
        
        // Clean up (although this won't be reached in normal operation)
        delete server;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Error: Unknown exception occurred" << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
