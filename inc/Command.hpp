/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 05:00:32 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/23 09:59:23 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <poll.h>
#include <fcntl.h>
#include <unistd.h>
# include <sys/types.h>

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

#include "Client.hpp"
#include "Server.hpp"

class Client;
class Server;

class Command
{
    public:
        std::map<std::string, void (Command::*)(Client *client, std::vector<std::string> args, Server *server)> _commands;
        Command();
        ~Command();
        void nick(Client *client, std::vector<std::string> args, Server *server);
        void user(Client *client, std::vector<std::string> args, Server *server);
        void join(Client *client, std::vector<std::string> args, Server *server);
        void part(Client *client, std::vector<std::string> args, Server *server);
        void privmsg(Client *client, std::vector<std::string> args, Server *server);
        void notice(Client *client, std::vector<std::string> args, Server *server);
        void quit(Client *client, std::vector<std::string> args, Server *server);
        void list(Client *client, std::vector<std::string> args, Server *server);
        void who(Client *client, std::vector<std::string> args, Server *server);
        void kick(Client *client, std::vector<std::string> args, Server *server);
        void mode(Client *client, std::vector<std::string> args, Server *server);
        void ping(Client *client, std::vector<std::string> args, Server *server);
        void pong(Client *client, std::vector<std::string> args, Server *server);
        void pass(Client *client, std::vector<std::string> args, Server *server);
        void names(Client *client, std::vector<std::string> args, Server *server);
        void topic(Client *client, std::vector<std::string> args, Server *server);
        void invite(Client *client, std::vector<std::string> args, Server *server);
        void execute(Client *client, std::vector<std::string> args, std::string command, Server *server);
};



#endif