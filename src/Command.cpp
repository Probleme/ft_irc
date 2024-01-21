/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aer-raou <aer-raou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 05:01:11 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/21 14:01:32 by aer-raou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
 

Command::~Command()
{
}

Command::Command()
{
    _commands["PASS"] = &Command::pass;
    _commands["NICK"] = &Command::nick;
    _commands["USER"] = &Command::user;
    _commands["KICK"] = &Command::kick;
    _commands["INVITE"] = &Command::invite;
    _commands["TOPIC"] = &Command::topic;
    _commands["MODE"] = &Command::mode;
    _commands["JOIN"] = &Command::join;
    _commands["PART"] = &Command::part;
    _commands["PRIVMSG"] = &Command::privmsg;
    _commands["QUIT"] = &Command::quit;
    _commands["LIST"] = &Command::list;
    _commands["WHO"] = &Command::who;
    _commands["PING"] = &Command::ping;
    _commands["PONG"] = &Command::pong;
    _commands["NOTICE"] = &Command::notice;
    _commands["/bot"] = &Command::bot;
    _commands["NAMES"] = &Command::names;
}

void Command::execute(Client *client, std::vector<std::string> args, std::string command, Server *server)
{
    (this->*_commands[command])(client, args, server);
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
                    return true;
            }
        }
    }
    return false;
}

void SetModeAndSandMessage(Client *client, std::string args, std::vector<Channel *>::iterator it, char c, int flag)
{
    if (flag == 1)
    {
        (*it)->setMode((*it)->getMode() + c);
        client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args, (*it)->getMode(), ' '));
        client->sendMessage();
    }
    else
    {
        (*it)->setMode((*it)->getMode().erase((*it)->getMode().find(c), 1));
        client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args, (*it)->getMode(), ' '));
        client->sendMessage();
    }
}

void SearchUserInChannel(Client *client, Channel *channel)
{
    std::vector<Client *> clients = channel->getClients();
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if ((*it)->getUsername() == client->getUsername())
            return;
    }
    client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel->getName()));
}

void Command::pass(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() != 1)
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), client->getCommand()));
    else if (args[0] != server->getPassword())
        client->reply(ERR_PASSWDMISMATCH(client->getNickname()));
    else if (client->getPassword() == true)
        client->reply(ERR_ALREADYREGISTERED(client->getNickname()));
    else if (args[0] == server->getPassword() && client->getPassword() == false)
    {
        client->setPassword(true);
        client->reply("Password accepted you have registered");
    }
}

void Command::nick(Client *client, std::vector<std::string> args, Server *server)
{
    std::vector<Client *> users = server->getUsers();
    if (args.size() == 0)
    {
        client->reply(ERR_NONICKNAMEGIVEN(client->getNickname()));
        return;
    }
    if (args[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890[]{}\\|") != std::string::npos ||
            args[0].empty() || args[0].find_first_of("0123456789") == 0)
    {
        client->reply(ERR_ERRONEUSNICKNAME(client->getNickname(), client->getCommand()));
        return;
    }
    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
    {
        for (std::vector<Client *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            if ((*it2)->getNickname() == *it)
            {
                client->reply(ERR_NICKNAMEINUSE(client->getNickname()));
                return;
            }
        }
    }
    std::string old_nickname = client->getNickname();
    client->setNickname(args[0]);
    client->reply(RPL_NICK(old_nickname, client->getUsername(), client->getHostname(), client->getNickname()));
}

void Command::user(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    if (client->isRegistered() && client->getUsername() != "" && client->getRealname() != "")
    {
        client->reply(ERR_ALREADYREGISTERED(client->getNickname()));
        return;
    }
    if (args.size() < 4 || args[0][0] == '\0')
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), client->getCommand()));
        return;
    }
    std::string realname;
    if (args.size() > 4 && args[3][0] == ':')
    {
        for (std::vector<std::string>::iterator it = args.begin() + 3; it != args.end(); it++)
            realname += *it + " ";
        client->setRealname(realname);
        return;
    }
    else if (args[3][0] != ':')
        realname = args[3];
    client->setUsername(args[0]);
    client->setRealname(args[3]);
}

void Command::invite(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() != 2)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "INVITE"));
        return;
    }
    std::vector<Channel *> channels = server->getChannels();
    if (channels.size() == 0)
    {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]));
        return;
    }
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        std::vector<Client *> clients = (*it)->getClients();
        for (std::vector<Client *>::iterator it2 = clients.begin(); it2 != clients.end(); it2++)
        {
            if ((*it2)->getNickname() == args[0])
            {
                client->reply(RPL_INVITING(client->getNickname(), args[0], args[1]));
                return;
            }
        }
        if ((*it)->getName() == args[1])
        {
            SearchUserInChannel(client, *it);
            if ((*it)->getMode().find('i') != std::string::npos)
            {
                client->reply(ERR_INVITEONLYCHAN(client->getNickname(), args[1]));
                return;
            }
            if ((*it)->CheckClientIsOperator(client->getNickname()))
            {
                client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), args[1]));
                return;
            }
            client->reply(RPL_INVITING(client->getNickname(), args[0], args[1]));
            return;
        }
    }
}

void Command::kick(Client *client, std::vector<std::string> args, Server *server)
{
    (void)client;
    if (args.size() != 2)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"));
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
                    if ((*it)->getMode().find('o') == std::string::npos)
                    {
                        client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), args[0]));
                        return;
                    }
                    (*it)->removeClient(*it2);
                    client->reply("kicked " + args[1] + " from channel " + args[0]);
                    return;
                }
                else
                {
                    client->reply(ERR_USERNOTINCHANNEL(client->getNickname(), args[1], args[0]));
                    return;
                }
            }
        }
        else
        {
            client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args[0]));
            return;
        }
    }
}

void Command::topic(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() != 3)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
        return;
    }
    std::vector<Channel *> channels = server->getChannels();
    if (channels.size() == 0)
    {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args[0]));
        return;
    }
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if ((*it)->getName() == args[0])
        {
            SearchUserInChannel(client, *it);
            if (args.size() == 1)
            {
                if ((*it)->getTopic() == "")
                {
                    (*it)->setTopicTime(server->dateString());
                    client->reply(RPL_NOTOPIC(client->getNickname(), args[0]));
                    client->reply(RPL_TOPICWHOTIME(client->getUsername(), args[0], client->getNickname(), (*it)->getTopicTime()));
                    return;
                }
                else
                {
                    (*it)->setTopicTime(server->dateString());
                    client->reply(RPL_TOPIC(client->getNickname(), args[0], (*it)->getTopic()));
                    client->reply(RPL_TOPICWHOTIME(client->getUsername(), args[0], client->getNickname(), (*it)->getTopicTime()));
                    return;
                }
            }
            else
            {
                server->checkClientPrivilege(client, *it);
                if (args[1] == ":")
                {
                    (*it)->setTopic("");
                    (*it)->setTopicTime(server->dateString());
                    client->reply(RPL_TOPIC(client->getNickname(), args[0], (*it)->getTopic()));
                    client->reply(RPL_TOPICWHOTIME(client->getUsername(), args[0], client->getNickname(), (*it)->getTopicTime()));
                }
                else if (args[1] != "" && args[1][0] != ':')
                {
                    std::string topic = args[1].substr(1);
                    for (std::vector<std::string>::iterator it = args.begin() + 3; it != args.end(); it++)
                        topic += *it + " ";
                    (*it)->setTopic(topic);
                    (*it)->setTopicTime(server->dateString());
                    client->reply(RPL_TOPIC(client->getNickname(), args[0], (*it)->getTopic()));
                    client->reply(RPL_TOPICWHOTIME(client->getUsername(), args[0], client->getNickname(), (*it)->getTopicTime()));
                    return;
                }
            }
        }
    }
}

void Command::mode(Client *client, std::vector<std::string> args, Server *server)
{
   if (args.size() == 0)
   {
        client->setMessage(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
        client->sendMessage();
       return;
   }
    if (args[0][0] == '#')
    {
        std::vector<Channel *> channels = server->getChannels();
        for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            if ((*it)->getName() == args[0])
            {
                if ((*it)->CheckClientIsOperator(client->getNickname()) == false)
                {
                    client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), args[0]));
                    return;
                }
                SearchUserInChannel(client, *it);
                if (args.size() == 1)
                {
                    client->reply(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode(), ' '));
                    return;
                }
                int flag = 1;
                for (int i = 0; args[1][i]  ; i++)
                {
                    if (args[1][i] == '+')
                        flag = 1;
                    else if (args[1][i] == '-')
                        flag = 2;
                    else if (args[1][i] == 'i')
                    {
                        if (flag == 1 && (*it)->getMode().find('i') == std::string::npos)
                            SetModeAndSandMessage(client, args[0], it, 'i', 1);
                        else if (flag == 2 && (*it)->getMode().find('i') != std::string::npos)
                            SetModeAndSandMessage(client, args[0], it, 'i', 2);
                    }
                    else if (args[1][i] == 't')
                    {
                        if (flag == 1 && (*it)->getMode().find('t') == std::string::npos)
                        {
                            (*it)->setTopicTime(std::to_string(time(0)));
                            (*it)->setTopic("");
                            SetModeAndSandMessage(client, args[0], it, 't', 1);
                        }
                        else if (flag == 2 && (*it)->getMode().find('t') != std::string::npos)
                        {
                            (*it)->setTopicTime(std::to_string(time(0)));
                            (*it)->setTopic("Default topic");
                            SetModeAndSandMessage(client, args[0], it, 't', 2);
                        }
                    }
                    else if (args[1][i] == 'k')
                    {
                        if (args.size() == 2)
                        {
                            client->reply(ERR_INVALIDMODEPARAM(client->getNickname(), args[0], args[1][i], "<key>"));
                            if (!args[1][i + 1])
                                return;
                            continue;
                            // i++;
                            // goto label;
                        }
                        else if (flag == 1 && (*it)->getMode().find('k') == std::string::npos)
                            SetModeAndSandMessage(client, args[0], it, 'k', 1);
                        else if (flag == 2 && (*it)->getMode().find('k') != std::string::npos)
                        {
                            if (args[2] != (*it)->getChannelKey() || args[2] == "")
                                client->reply(ERR_BADCHANNELKEY(client->getNickname(), args[0]));
                            else
                                SetModeAndSandMessage(client, args[0], it, 'k', 2);
                            return;
                        }
                    }
                    else if (args[1][i] == 'o')
                    {
                        if (flag == 1 && (*it)->CheckClientIsOperator(client->getNickname()))
                        {
                            (*it)->AddChannelOperator((*it)->getClient(args[2]));
                            client->reply(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode(), "+o " + args[2]));
                        }
                        else if (flag == 2 &&(*it)->CheckClientIsOperator(client->getNickname()))
                        {
                            (*it)->RemoveChannelOperator((*it)->getClient(args[2]));
                            client->reply(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode(), "-o " + args[2]));
                        }
                    }
                    else if (args[1][i] == 'l')
                    {
                        if (args.size() == 2 && flag == 1)
                        {
                            client->reply(ERR_INVALIDMODEPARAM(client->getNickname(), args[0], args[1][i], "<limit>"));
                            if (!args[1][i + 1])
                                return;
                            continue;
                            // i++;
                            // goto label;
                        }
                        if (flag == 1 && (*it)->getMode().find('l') == std::string::npos)
                            SetModeAndSandMessage(client, args[0], it, 'l', 1);
                        else if (flag == 2 && (*it)->getMode().find('l') != std::string::npos)
                            SetModeAndSandMessage(client, args[0], it, 'l', 2);
                    }
                    else
                        client->reply(ERR_UNKNOWNMODE(client->getNickname(), args[1][i]));
                }
            }
            else if ((*it) == channels.back() && (*it)->getName() != args[0])
            {
                client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args[0]));
                return;
            }
        }
    }
    else
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args[0]));
}

void Command::join(Client *client, std::vector<std::string> args, Server *server)
{
   if (args.size() == 0 || args.size() > 2)
   {
       client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), client->getCommand()));
       return;
   }
    std::vector<std::string> channel_names = client->split(args[0], ',');
    std::vector<std::string> keys;
    if (args.size() == 2)
        keys = client->split(args[1], ',');
    else
        keys.push_back("");
    std::vector<Channel *> channels = server->getChannels();
    int n = 0;
    Channel *new_channel = nullptr;
    for (std::vector<std::string>::iterator it = channel_names.begin(); it != channel_names.end(); it++)
    {
        bool valid_channel_name = true;
        if (it->find_first_not_of("#abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890[]{}\\|") != std::string::npos ||
                it->empty() || it->find_first_of("0123456789") == 0)
            valid_channel_name = false;
        if (it->find_first_of("#") != 0 || it->find('#') == std::string::npos)
            valid_channel_name = false;
        if (!valid_channel_name)
        {
            client->reply(ERR_BADCHANNELKEY(client->getNickname(), *it));
            continue;
        }
        if (check_if_user_is_in_channel(client, *it, channels))
            continue;
        Channel *exist_channel = nullptr;
        for (std::vector<Channel *>::iterator it2 = channels.begin(); it2 != channels.end(); it2++)
        {
            if ((*it2)->getName() == *it)
            {
                exist_channel = *it2;
                break;
            }
        }
        if (!exist_channel)
        {    
            new_channel = new Channel(*it, client);
            new_channel->setChannelCreationTime(time(0));
            server->addChannel2(new_channel);
            if (keys[n].size() > 0)
            {
                new_channel->setMode(new_channel->getMode() + "k");
                new_channel->setChannelKey(keys[n]);
                client->reply(RPL_NOTOPIC(client->getNickname(), *it));
                client->reply(RPL_NAMREPLY(client->getNickname(), *it, client->getNickname()));
                client->reply(RPL_ENDOFNAMES(client->getNickname(), *it));
            }
            else if (keys[n].size() == 0)
            {
                client->reply(RPL_NOTOPIC(client->getNickname(), *it));
                client->reply(RPL_NAMREPLY(client->getNickname(), *it, client->getNickname()));
                client->reply(RPL_ENDOFNAMES(client->getNickname(), *it));
            }
            if (keys[n] == keys.back())
            {
                keys[0] = "";
                n = 0;
            }
            else if (keys[n].size() > 0)
                n++;
            continue;
        }
        for (std::vector<Channel *>::iterator it2 = channels.begin(); it2 != channels.end(); it2++)
        {
            if ((*it2)->getName() == *it)
            {
                exist_channel = *it2;
                break;
            }
        }
        if (exist_channel->getClients().size() >= (size_t)exist_channel->getChannelLimit())
        {
            client->reply(ERR_CHANNELISFULL(client->getNickname(), *it));
            continue;
        }
        if (exist_channel->getMode().find('i') != std::string::npos)
        {
            client->reply(ERR_INVITEONLYCHAN(client->getNickname(), *it));
            continue;
        }
        else if (exist_channel->getMode().find('k') != std::string::npos)
        {
            if (keys[n] != exist_channel->getChannelKey())
            {
                client->reply(ERR_BADCHANNELKEY(client->getNickname(), *it));
                continue;
            }
            n++;
        }
        else if (exist_channel->getMode().find('l') != std::string::npos)
        {
            if (exist_channel->getClients().size() >= (size_t)exist_channel->getUserLimit())
            {
                client->reply(ERR_CHANNELISFULL(client->getNickname(), *it));
                continue;
            }
        }
        if (exist_channel->getTopic() != "")
        {
            exist_channel->addClient(client);
            client->reply(RPL_TOPIC(client->getNickname(), *it, exist_channel->getTopic()));
            client->reply(RPL_TOPICWHOTIME(client->getNickname(), *it, exist_channel->getTopic(), exist_channel->getTopicTime()));
            client->reply(RPL_NAMREPLY(client->getNickname(), *it, client->getNickname()));
            client->reply(RPL_ENDOFNAMES(client->getNickname(), *it));
            server->sendToAllClientsInChannel(JOIN_SUCC(client->getNickname(), client->getUsername(), client->getHostname(), exist_channel->getName()), exist_channel,client);
            client->reply(JOIN_SUCC(client->getNickname(), client->getUsername(), client->getHostname(), exist_channel->getName()));
        }
        else
        {
            exist_channel->addClient(client);
            client->reply(RPL_NOTOPIC(client->getNickname(), *it));
            client->reply(RPL_NAMREPLY(client->getNickname(), *it, client->getNickname()));
            client->reply(RPL_ENDOFNAMES(client->getNickname(), *it));
            server->sendToAllClientsInChannel(JOIN_SUCC(client->getNickname(), client->getUsername(), client->getHostname(), exist_channel->getName()), exist_channel,client);
            client->reply(JOIN_SUCC(client->getNickname(), client->getUsername(), client->getHostname(), exist_channel->getName()));
        }
    }
}

void Command::part(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() < 1 || args.size() > 2)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));
        return;
    }
    std::string msg;
    if (args.size() == 2)
        msg = args[1];
    else
        msg = "";
    std::vector<std::string> channelsToPart = client->split(args[0], ',');
    std::vector<Channel *> channels = server->getChannels();
    for (std::vector<std::string>::iterator it = channelsToPart.begin(); it != channelsToPart.end(); it++)
    {
        for (std::vector<Channel *>::iterator it2 = channels.begin(); it2 != channels.end(); it2++)
        {
            if ((*it2)->getName() == *it)
            {
                SearchUserInChannel(client, *it2);
                (*it2)->removeClient(client);
                client->reply("left channel " + *it);
                if (msg != "")
                    client->reply(msg);
                return;
            }
        }
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), *it));
    }
    client->setMessage(ERR_NOSUCHCHANNEL(client->getNickname(), args[0]));
    client->sendMessage();
}

void Command::names(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() == 0)
    {
        std::vector<Client *> clients = server->getUsers();
        for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
            client->reply(RPL_NAMREPLY(client->getNickname(), "=", (*it)->getNickname()));
        return;
    }
    std::vector<Channel *> serverChannels = server->getChannels();
    std::vector<std::string> channels = client->split(args[0], ',');
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        for (std::vector<Channel *>::iterator it2 = serverChannels.begin(); it2 != serverChannels.end(); it2++)
        {
            if ((*it2)->getName() == *it)
            {
                std::vector<Client *> clients = (*it2)->getClients();
                for (std::vector<Client *>::iterator it3 = clients.begin(); it3 != clients.end(); it3++)
                {
                    client->reply(RPL_NAMREPLY(client->getNickname(), *it, (*it3)->getNickname()));
                }
            }
            else if ((*it2) == serverChannels.back() && (*it2)->getName() != *it)
            {
                client->reply(RPL_ENDOFNAMES(client->getNickname(), *it));
            }
        }
    }
}

void ListConditions(Client *client, std::vector<Channel *> channels, std::string value, std::string args)
{
    time_t now = time(0);
    if (value.find_first_not_of("0123456789") != std::string::npos)
    {
        client->reply("invalid arguments");
        return;
    }
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        time_t channel_creation_time = (*it)->getChannelCreationTime();
        double diff = difftime(now, channel_creation_time);
        double minutes = diff / 60;
        std::string size = std::to_string((*it)->getClients().size());
        if (args[1] == '>')
        {
            if (minutes >= std::stod(value))
            {
                std::string message = RPL_LIST(client->getNickname(), (*it)->getName(), size, (*it)->getTopic());
            }
        }
        else if (args[1] == '<')
        {
            if (minutes <= std::stod(value))
            {
                client->reply(RPL_LIST(client->getNickname(), (*it)->getName(), size, (*it)->getTopic()));
            }
        }
    }
}

void Command::list(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() == 0)
    {
        std::vector<Channel *> channels = server->getChannels();
        for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            std::string size = std::to_string((*it)->getClients().size());
            client->reply(RPL_LIST(client->getNickname(), (*it)->getName(), size, (*it)->getTopic()));
        }
    }
    if (args.size() == 1)
    {
        std::vector<Channel *> channels = server->getChannels();
        std::vector<std::string> conditions;
        if (args[0][0] == '#')
        {
            conditions = client->split(args[0], ',');
            for (std::vector<std::string>::iterator it = conditions.begin(); it != conditions.end(); it++)
            {
                for (std::vector<Channel *>::iterator it2 = channels.begin(); it2 != channels.end(); it2++)
                {
                    if ((*it2)->getName() == *it)
                    {
                        std::string size = std::to_string((*it2)->getClients().size());
                        client->reply(RPL_LIST(client->getNickname(), (*it2)->getName(), size, (*it2)->getTopic()));
                    }
                    else if ((*it2) == channels.back() && (*it2)->getName() != *it)
                    {
                        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), *it));
                    }
                }
            }
        }
        else
        {
            if (args[0][0] == 'C')
            {
                std::string value = args[0].substr(2);
                ListConditions(client, channels, value, args[0]);
            }
            else if (args[0][0] == 'U')
            {
                std::string value = args[0].substr(2);
                if (value.find_first_not_of("0123456789") != std::string::npos || value.empty())
                {
                    client->reply("invalid condition" + args[0]);
                    return;
                }
                for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
                {
                    if (args[0][1] == '>')
                    {
                        if ((*it)->getClients().size() >= std::stod(value))
                        {
                            std::string msg = (*it)->getName() + ": Connected clients : " + std::to_string((*it)->getClients().size());
                            client->reply(msg);
                        }
                    }
                    else if (args[0][1] == '<')
                    {
                        if ((*it)->getClients().size() <= std::stod(value))
                        {
                            std::string msg = (*it)->getName() + ": Connected clients : " + std::to_string((*it)->getClients().size());
                            client->reply(msg);
                        }
                    }
                }
            }
            else if (args[0][0] == '>' || args[0][0] == '<')
            {
                std::string value = args[0].substr(1);
                ListConditions(client, channels, value, "C" + args[0]);
            }
        }
    }
}

void Command::privmsg(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() < 2)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), client->getCommand()));
        return;
    }
    std::string target = args[0];
    std::string msg;
    for (std::vector<std::string>::iterator it = args.begin() + 1; it != args.end(); it++)
        msg += *it + " ";
    if (msg.empty())
    {
        client->reply(ERR_NOTEXTTOSEND(client->getNickname()));
        return;
    }
    if (target.empty())
    {
        client->reply(ERR_NORECIPIENT(client->getNickname(), client->getCommand()));
        return;
    }
    std::vector<std::string> targets = client->split(target, ',');
    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
    {
        if (it->find_first_not_of("#abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890[]{}\\|") != std::string::npos ||
                it->empty() || it->find_first_of("0123456789") == 0)
        {
            client->reply(ERR_NOSUCHNICK(client->getNickname(), *it));
            continue;
        }
        if (it->find_first_of("#") == 0)
        {
            std::vector<Channel *> channels = server->getChannels();
            for (std::vector<Channel *>::iterator it2 = channels.begin(); it2 != channels.end(); it2++)
            {
                if ((*it2)->getName() == *it)
                {
                    std::vector<Client *> clients = (*it2)->getClients();
                    for (std::vector<Client *>::iterator it3 = clients.begin(); it3 != clients.end(); it3++)
                    {
                        if ((*it3)->getUsername() == client->getUsername())
                            continue;
                        (*it3)->reply(PRIVMSG(client->getNickname(), client->getUsername(), client->getHostname(), *it, msg));
                    }
                    break;
                }
                else if ((*it2) == channels.back() && (*it2)->getName() != *it)
                {
                    client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), *it));
                    break;
                }
            }
            continue;
        }
        int idx = 0;
        std::vector<Client *> users = server->getUsers();
        for (std::vector<Client *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            if ((*it2)->getNickname() == *it)
            {
                (*it2)->reply(PRIVMSG(client->getNickname(), client->getUsername(), client->getHostname(), *it, msg));
                idx++;
                break;
            }
            else if ((*it2) == users.back() && (*it2)->getNickname() != *it)
            {
                client->reply(ERR_NOSUCHNICK(client->getNickname(), *it));
                break;
            }
        }
    }
}

void Command::notice(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() < 2)
    {
        return;
    }
    std::string target = args[0];
    std::string msg;
    for (std::vector<std::string>::iterator it = args.begin() + 1; it != args.end(); it++)
        msg += *it + " ";
    if (msg.empty())
    {
        return;
    }
    if (target.empty())
    {
        return;
    }
    std::vector<std::string> targets = client->split(target, ',');
    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
    {
        if (it->find_first_not_of("#abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890[]{}\\|") != std::string::npos ||
                it->empty() || it->find_first_of("0123456789") == 0)
        {
            continue;
        }
        if (it->find_first_of("#") == 0)
        {
            std::vector<Channel *> channels = server->getChannels();
            for (std::vector<Channel *>::iterator it2 = channels.begin(); it2 != channels.end(); it2++)
            {
                if ((*it2)->getName() == *it)
                {
                    std::vector<Client *> clients = (*it2)->getClients();
                    for (std::vector<Client *>::iterator it3 = clients.begin(); it3 != clients.end(); it3++)
                    {
                        if ((*it3)->getUsername() == client->getUsername())
                            continue;
                        (*it3)->reply(NOTICE(client->getNickname(), client->getUsername(), client->getHostname(), *it, msg));
                    }
                    break;
                }
                else if ((*it2) == channels.back() && (*it2)->getName() != *it)
                {
                    break;
                }
            }
            continue;
        }
        int idx = 0;
        std::vector<Client *> users = server->getUsers();
        for (std::vector<Client *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            if ((*it2)->getNickname() == *it)
            {
                (*it2)->reply(NOTICE(client->getNickname(), client->getUsername(), client->getHostname(), *it, msg));
                idx++;
                break;
            }
            else if ((*it2) == users.back() && (*it2)->getNickname() != *it)
            {
                break;
            }
        }
    }
}

void Command::quit(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    std::string msg;
    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
        msg += " " + (*it);
    std::vector<Channel *> channels = server->getChannels();
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        std::vector<Client *> clients = (*it)->getClients();
        for (std::vector<Client *>::iterator it2 = clients.begin(); it2 != clients.end(); it2++)
        {
            if ((*it2)->getNickname() == client->getNickname())
            {
                (*it)->removeClient(client);
                server->sendToAllClientsInChannel(QUIT_MSG(client->getNickname(), client->getUsername(), client->getHostname(), msg), *it, client);
            }
        }
    }
    client->reply(QUIT_MSG(client->getNickname(), client->getUsername(), client->getHostname(), msg));
    server->removeClient(client->getFd());
    server->setDescriptors();
}

void Command::who(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() != 1)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "WHO"));
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
                client->reply(RPL_WHOREPLY(client->getNickname(), args[0], (*it2)->getUsername(), (*it2)->getHostname(), server->getServerName(), (*it2)->getNickname(), (*it)->getMode(), (*it2)->getRealname()));
            }
            client->reply(RPL_ENDOFWHO(client->getNickname()));
            return;
        }
    }
    std::vector<Client *> users = server->getUsers();
    for (std::vector<Client *>::iterator it = users.begin(); it != users.end(); it++)
    {
        if ((*it)->getNickname() == args[0])
        {
            client->reply(RPL_WHOREPLY(client->getNickname(), args[0], (*it)->getUsername(), (*it)->getHostname(), server->getServerName(), (*it)->getNickname(), "", (*it)->getRealname()));
            client->reply(RPL_ENDOFWHO(client->getNickname()));
            return;
        }
    }
    client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args[0]));
}

void Command::ping(Client *client, std::vector<std::string> args, Server *server)
{
    std::vector<Client *> users = server->getUsers();
    if (args.size() != 1)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PING"));
        return;
    }
    else if (!args[0][0])
    {
        client->reply(ERR_NOORIGIN(client->getNickname()));
        return;
    }
    if (args[0] == "Problem_irc")
    {
        client->reply(PONG(client->getNickname(), args[0]));
        return;
    }
    for (std::vector<Client *> ::iterator it = users.begin(); it != users.end(); it++)
    {
        if ((*it)->getUsername() == args[0] || (*it)->getNickname() == args[0] || (*it)->getRealname() == args[0])
        {
            std::string msg = "PING " + args[0];
            client->reply(msg);
        }
    }
}
void Command::pong(Client *client, std::vector<std::string> args, Server *server)
{
    std::vector<Client *> users = server->getUsers();
    if (args.size() != 1)
        return;
    if (args[0] == "Problem_irc")
    {
        client->reply(PING(client->getNickname()));
        return;
    }
    for (std::vector<Client *> ::iterator it = users.begin(); it != users.end(); it++)
    {
        if ((*it)->getUsername() == args[0] || (*it)->getNickname() == args[0] || (*it)->getRealname() == args[0])
        {
            client->reply(PONG(client->getNickname(), args[0]));
        }
    }
}
