/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 05:01:11 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/10 11:31:33 by ataouaf          ###   ########.fr       */
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

void Command::execute(Client *client, std::vector<std::string> args, std::string command)
{
    if (args.empty())
    {
        client->reply("invalid arguments");
        return; 
    }
    (this->*_commands[command])(client, args);
}

void Command::nick(Client *client, std::vector<std::string> args)
{
    if (args.size() != 1)
    {
        client->reply("invalid arguments");
        return;
    }
    client->setNickname(args[0]);
    client->welcome();
}

void Command::user(Client *client, std::vector<std::string> args)
{
    if (args.size() != 3)
    {
        client->reply("invalid arguments");
        return;
    }
    // if (!client->isRegistered())
    // {
    //     client->reply("already registered");
    //     return;
    // }
    client->setUsername(args[0]);
    client->setRealname(args[2].substr(1));
    client->welcome();
}

void Command::join(Client *client, std::vector<std::string> args)
{
    (void)client;
    (void)args;
}

void Command::part(Client *client, std::vector<std::string> args)
{
    (void)client;
    (void)args;
}

void Command::privmsg(Client *client, std::vector<std::string> args)
{
    (void)client;
    (void)args;
}

void Command::notice(Client *client, std::vector<std::string> args)
{
    (void)client;
    (void)args;
}

void Command::quit(Client *client, std::vector<std::string> args)
{
    (void)client;
    (void)args;
}

void Command::list(Client *client, std::vector<std::string> args)
{
    (void)client;
    (void)args;
}

void Command::who(Client *client, std::vector<std::string> args)
{
    (void)client;
    (void)args;
}

void Command::kick(Client *client, std::vector<std::string> args)
{
    (void)client;
    (void)args;
}

void Command::mode(Client *client, std::vector<std::string> args)
{
    (void)client;
    (void)args;
}

void Command::ping(Client *client, std::vector<std::string> args)
{
    (void)client;
    (void)args;
}

void Command::pong(Client *client, std::vector<std::string> args)
{
    (void)client;
    (void)args;
}

void Command::pass(Client *client, std::vector<std::string> args)
{
    (void)client;
    (void)args;
}
