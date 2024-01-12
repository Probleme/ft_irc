/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 05:00:32 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/10 11:24:15 by ataouaf          ###   ########.fr       */
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
        std::map<std::string, void (Command::*)(Client *client, std::vector<std::string> args)> _commands;
        Command();
        ~Command();
        void nick(Client *client, std::vector<std::string> args);
        void user(Client *client, std::vector<std::string> args);
        void join(Client *client, std::vector<std::string> args);
        void part(Client *client, std::vector<std::string> args);
        void privmsg(Client *client, std::vector<std::string> args);
        void notice(Client *client, std::vector<std::string> args);
        void quit(Client *client, std::vector<std::string> args);
        void list(Client *client, std::vector<std::string> args);
        void who(Client *client, std::vector<std::string> args);
        void kick(Client *client, std::vector<std::string> args);
        void mode(Client *client, std::vector<std::string> args);
        void ping(Client *client, std::vector<std::string> args);
        void pong(Client *client, std::vector<std::string> args);
        void pass(Client *client, std::vector<std::string> args);
        void execute(Client *client, std::vector<std::string> args, std::string command);
        
};



#endif