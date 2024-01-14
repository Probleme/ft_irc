/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aer-raou <aer-raou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 05:01:11 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/14 11:15:06 by aer-raou         ###   ########.fr       */
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
bool check_if_user_is_in_channel(Client *client, std::string channel_name, std::vector<Channel *> channels)
{
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if ((*it)->getName() == channel_name)
        {
            std::vector<Client *> clients = (*it)->getClients();
            for (std::vector<Client *>::iterator it2 = clients.begin(); it2 != clients.end(); it2++)
            {
                if ((*it2)->getNickname() == client->getNickname())
                {
                    client->reply("already in channel " + channel_name);
                    return true;
                }
            }
        }
    }
    return false;
}
void Command::join(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() != 1)
    {
        client->reply("invalid arguments");
        return;
    }
    std::vector<Channel *> channels = server->getChannels();
    if (check_if_user_is_in_channel(client, args[0], channels))
        return;
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if ((*it)->getName() == args[0])
        {
            (*it)->addClient(client);
            client->reply("Joined channel " + args[0]);
            return;
        }
    }
    Channel *channel = new Channel(args[0], client);
    server->addChannel(channel, client);
    client->reply("Created channel " + args[0]);
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
    if (args.size() == 0)
    {
        std::vector<Channel *> channels = server->getChannels();
        for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            std::string msg = (*it)->getName() + ": Connected clients : " + std::to_string((*it)->getClients().size());
            client->reply(msg);
        }
    }
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
    if (args.size() != 2)
    {
        client->reply("Invalid arguments");
        return;
    }
    std::vector<Channel *> channels = server->getChannels();
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if ((*it)->getName() == args[0])
        {
            std::vector<Client *> clients = (*it)->getClients();
            for (std::vector<Client *>::iterator it2 = clients.begin(); it2 != clients.end(); it2++)
            {
                if ((*it2)->getNickname() == args[1])
                {
                    (*it)->removeClient(*it2);
                    client->reply("kicked " + args[1] + " from channel " + args[0]);
                    return;
                }
            }
        }
        else
        {
            client->reply("channel " + args[0] + " doesn't exist");
            return;
        }
    }
}

void Command::mode(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    (void)client;
    (void)args;
}

void Command::ping(Client *client, std::vector<std::string> args, Server *server)
{
    std::vector<Client *> users = server->getUsers();
    if (args.size() != 1)
    {
        client->reply("invalid arguments");
        client->reply("PING <server or nickname>");
        return;
    }
    if (args[0] == "Problem_irc")
    {
        client->reply("PONG Problem_irc");
        return;
    }
    for (std::vector<std::string >::iterator it = args.begin(); it != args.end(); it++)
    {
        for (std::vector<Client *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            if ((*it2)->getUsername() == *it || (*it2)->getNickname() == *it || (*it2)->getRealname() == *it)
            {
                std::string msg = "PONG " + *it;
                client->reply(msg);
            }
        }
    }
}

void Command::pong(Client *client, std::vector<std::string> args, Server *server)
{
    std::vector<Client *> users = server->getUsers();
    if (args.size() != 1)
    {
        client->reply("invalid arguments");
        client->reply("PONG <server or nickname>");
        return;
    }
    if (args[0] == "Problem_irc")
    {
        client->reply("PING Problem_irc");
        return;
    }
    for (std::vector<std::string >::iterator it = args.begin(); it != args.end(); it++)
    {
        for (std::vector<Client *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            if ((*it2)->getUsername() == *it || (*it2)->getNickname() == *it || (*it2)->getRealname() == *it)
            {
                std::string msg = "PING " + *it;
                client->reply(msg);
            }
        }
    }
}

void Command::pass(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    (void)client;
    (void)args;
}
