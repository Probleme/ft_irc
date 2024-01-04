/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 16:26:16 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/04 22:06:07 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/server.hpp"
#include <unistd.h>

Server::Server()
{
}

Server::Server(std::string password ,int port) : _server_name("default"),_password(password), _port(port),  _socket(0)
{
}

Server::Server(const Server &s)
{
    *this = s;
}

Server::~Server()
{}

Server &Server::operator=(const Server &s)
{
    if (this != &s)
    {
        this->_server_name = s._server_name;
        this->_start_time = s._start_time;
        this->_password = s._password;
        this->_port = s._port;
        this->_socket = s._socket;
    }
    return (*this);
}

void Server::run()
{
    int opt = 1;
    int addrlen = sizeof(_client_address);
    char buffer[BUFFER_SIZE] = {0};
    std::string hello = "Hello from server";
    std::string message;

    if ((_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) == 0)
    {
        std::cerr << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR , (char *)&opt, sizeof(opt)) < 0)
    {
        std::cerr << "setsockopt failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    _client_address.sin6_family = AF_INET6;
    _client_address.sin6_addr = in6addr_any;
    _client_address.sin6_port = htons(_port);
    if (bind(_socket, (struct sockaddr *)&_client_address, sizeof(_client_address)) < 0)
    {
        std::cerr << "bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(_socket, 3) < 0)
    {
        std::cerr << "listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        std::cout << "Waiting for a connection..." << std::endl;
        if ((_socket = accept(_socket, (struct sockaddr *)&_client_address, (socklen_t*)&addrlen)) < 0)
        {
            std::cerr << "accept failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << "Connection accepted" << std::endl;
        send(_socket, hello.c_str(), hello.length(), 0);
        std::cout << "Hello message sent" << std::endl;
        read(_socket, buffer, BUFFER_SIZE);
        std::cout << buffer << std::endl;
        message = "Hello from server";
        send(_socket, message.c_str(), message.length(), 0);
        std::cout << "Hello message sent" << std::endl;
        close(_socket);
    }
}
