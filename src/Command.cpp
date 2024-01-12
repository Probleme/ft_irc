/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aer-raou <aer-raou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 05:01:11 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/12 20:46:53 by aer-raou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"

Command::~Command() {
}

Command::Command() {
    _commands["NICK"] = &Command::nick;
    _commands["USER"] = &Command::user;
    _commands["JOIN"] = &Command::join;
    _commands["PART"] = &Command::part;
    _commands["PRIVMSG"] = &Command::privmsg;
    _commands["INVITE"] = &Command::notice;
    _commands["QUIT"] = &Command::quit;
    _commands["LIST"] = &Command::list;
    _commands["WHO"] = &Command::who;
    _commands["KICK"] = &Command::kick;
    _commands["MODE"] = &Command::mode;
    _commands["PING"] = &Command::ping;
    _commands["PONG"] = &Command::pong;
    _commands["NOTICE"] = &Command::notice;
    _commands["PASS"] = &Command::pass;
    
}

void Command::execute(Client *client, std::vector<std::string> args, std::string command, Server *server)
{
    // if (args.empty())
    // {
    //     client->reply("invalid arguments");
    //     return; 
    // }
    (this->*_commands[command])(client, args, server);
}

void Command::nick(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    if (args.size() != 1)
    {
        client->reply("invalid arguments");
        return;
    }
    client->setNickname(args[0]);
    client->welcome();
}

void Command::user(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    if (args.size() != 4)
    {
        client->reply("invalid arguments");
        return;
    }
    client->setUsername(args[0]);
    client->setRealname(args[3]);
    client->welcome();
    // if (!client->isRegistered())
    // {
    //     client->reply("already registered");
    //     return;
    // }
}

void Command::join(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    (void)client;
    (void)args;
}

void Command::part(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    (void)client;
    (void)args;
}

void Command::privmsg(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    (void)client;
    (void)args;
}

void Command::notice(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    (void)client;
    (void)args;
}

void Command::quit(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    std::string msg;
    for (std::vector<std::string >::iterator it = args.begin(); it != args.end(); it++)
        msg += it->c_str();
    client->setMessage(msg);
    client->sendMessage();
    close(client->getFd());
}

void Command::list(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    (void)client;
    (void)args;
}

void Command::who(Client *client, std::vector<std::string> args, Server *server)
{
    std::vector<Client *> users = server->getUsers();
    if (args.size() == 0)
    {
        for (std::vector<Client *>::iterator it = users.begin(); it != users.end(); it++)
        {
            std::string msg = (*it)->getNickname() + " " + (*it)->getUsername() + " " + (*it)->getHostname() + " " + (*it)->getServername();
            client->reply(msg);
        }
    }
    for (std::vector<std::string >::iterator it = args.begin(); it != args.end(); it++)
    {
        for (std::vector<Client *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            if ((*it2)->getUsername() == *it || (*it2)->getNickname() == *it || (*it2)->getRealname() == *it)
            {
                std::string msg = (*it2)->getNickname() + " " + (*it2)->getUsername() + " " + (*it2)->getHostname() + " " + (*it2)->getServername();
                client->reply(msg);
            }
        }
    }
}

void Command::kick(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    (void)client;
    (void)args;
}

void Command::mode(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    (void)client;
    (void)args;
}

void Command::ping(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    if (args.size() == 0)
    {
        client->reply("invalid arguments");
        return;
    }
    client->reply("PONG " + args[0]);
}

void Command::pong(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    (void)client;
    (void)args;
}

void Command::pass(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    (void)client;
    (void)args;
}
