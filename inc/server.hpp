/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 16:26:28 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/04 05:57:48 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>

# include "utils.hpp"

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# define BUFFER_SIZE 1024


class Server
{
    private:
        std::string _server_name; // server_name is a string of 32 characters
        std::string _password; // password is a string of 8 characters
        std::string _start_time; // start_time is a string of 32 characters
        int _port; // port is a 16-bit unsigned integer
        int _socket; // socket is an endpoint for communication
        struct sockaddr_in6 _client_address; // sockaddr_in is a structure containing an internet address
        std::vector<std::string> _channels; // channels is a list of channels
        std::map<std::string, std::string> _users; // users is a list of users
        std::map<std::string, std::string> _messages; // messages is a list of messages
    public:
        Server();
        Server(std::string password, int port);
        Server(const Server &s);
        ~Server();
        Server &operator=(const Server &s);

        void run();
};

# endif