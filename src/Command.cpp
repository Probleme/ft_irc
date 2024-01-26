/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aer-raou <aer-raou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 05:01:11 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/26 18:23:53 by aer-raou         ###   ########.fr       */
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
    _commands["NOTICE"] = &Command::notice;
    _commands["/bot"] = &Command::bot;
    _commands["NAMES"] = &Command::names;
    _commands["PING"] = &Command::ping;
    _commands["PONG"] = &Command::pong;
    _commands["pass"] = &Command::pass;
    _commands["nick"] = &Command::nick;
    _commands["user"] = &Command::user;
    _commands["kick"] = &Command::kick;
    _commands["invite"] = &Command::invite;
    _commands["topic"] = &Command::topic;
    _commands["mode"] = &Command::mode;
    _commands["join"] = &Command::join;
    _commands["part"] = &Command::part;
    _commands["privmsg"] = &Command::privmsg;
    _commands["quit"] = &Command::quit;
    _commands["list"] = &Command::list;
    _commands["who"] = &Command::who;
    _commands["notice"] = &Command::notice;
    _commands["names"] = &Command::names;
    _commands["ping"] = &Command::ping;
    _commands["pong"] = &Command::pong;
    
}

void Command::execute(Client *client, std::vector<std::string> args, std::string command, Server *server)
{
     if (command == "PRIVMSG" && args[0] == "bot")
    {
        command = "/bot";
        args.erase(args.begin());
        if (args.size() > 0 && args.at(0).at(0) == ':')
            args.at(0).erase(args.at(0).begin());
    }
    if (command == "JOIN" && server->flag == 0)
        server->flag = 1;
    if (command == "MODE" && server->flag == 1)
        return ;
    if ((command != "JOIN" && command != "join") && server->flag == 1)
        server->flag = 0;
    std::cout<< ":commanddd " << command << std::endl;
    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
        std::cout << ":args " << *it << std::endl;
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

void SetModeAndSandMessage(Client *client, std::string args, std::vector<Channel *>::iterator it, std::string str, int flag)
{
    if (flag == 1)
    {
        (*it)->setMode((*it)->getMode() + str);
        client->reply(MODE_MSG(client->getNickname(), client->getUsername(), client->getHostname(), args.at(0), str + " ", args.at(2)));
    }
    else
    {
        (*it)->setMode((*it)->getMode().erase((*it)->getMode().find(str), 1));
        client->reply(MODE_MSG(client->getNickname(), client->getUsername(), client->getHostname(), args.at(0), str + " ", args.at(2)));
        // client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args, (*it)->getMode(), ' '));
        // client->sendMessage();
    }
}

bool SearchUserInChannel(Client *client, Channel *channel)
{
    std::vector<Client *> clients = channel->getClients();
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if ((*it)->getUsername() == client->getUsername())
            return true;
    }
    client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel->getName()));
    return false;
}

void Command::pass(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() != 1)
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), client->getCommand()));
    else if (args.at(0) != server->getPassword())
        client->reply(ERR_PASSWDMISMATCH(client->getNickname()));
    else if (client->getPassword() == true)
        client->reply(ERR_ALREADYREGISTERED(client->getNickname()));
    else if (args.at(0) == server->getPassword() && client->getPassword() == false)
        client->setPassword(true);
}

void Command::nick(Client *client, std::vector<std::string> args, Server *server)
{
    std::vector<Client *> users = server->getUsers();
    if (args.size() == 0)
    {
        client->reply(ERR_NONICKNAMEGIVEN(client->getNickname()));
        return;
    }
    if (args.at(0).find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890[]{}\\|") != std::string::npos ||
            args.at(0).empty() || args.at(0).find_first_of("0123456789") == 0)
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
    client->setNickname(args.at(0));
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
    if (args.size() < 4 || args.at(0).at(0) == '\0')
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), client->getCommand()));
        return;
    }
    std::string realname;
    if (args.size() > 4 && args.at(3).at(0) == ':')
    {
        for (std::vector<std::string>::iterator it = args.begin() + 3; it != args.end(); it++)
            realname += *it + " ";
        client->setRealname(realname);
        return;
    }
    else if (args.at(3).at(0) != ':')
        realname = args.at(3);
    client->setUsername(args.at(0));
    client->setRealname(args.at(3));
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
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args.at(1)));
        return;
    }
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (args.at(1) == (*it)->getName())
        {
            Client *clientInvited = server->getClientByNickname(args.at(0));
            if (clientInvited == nullptr)
            {
                client->reply(ERR_NOSUCHNICK(client->getNickname(), args.at(0)));
                return;
            }
            if ((*it)->CheckClientIsOperator(client->getNickname()) == false)
            {
                client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), args.at(1)));
                return;
            }
            if (check_if_user_is_in_channel(clientInvited, args.at(1), channels))
            {
                client->reply(ERR_USERONCHANNEL(client->getNickname(), args.at(0), args.at(1)));
                return;
            }
            client->reply(RPL_INVITING(client->getNickname(), args.at(0), args.at(1)));
            clientInvited->reply(RPL_INVITING(client->getNickname(), args.at(0), args.at(1)));
            clientInvited->setIsInvited(true);
            return;
        }
    }
    client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args.at(1)));
}

void Command::kick(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() < 2)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"));
        return;
    }
    std::vector<std::string> usersToKick = client->split(args.at(1), ',');
    for (std::vector<std::string>::iterator it0 = usersToKick.begin(); it0 != usersToKick.end(); it0++)
    {
        std::vector<Channel *> channels = server->getChannels();
        for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            if ((*it)->getName() == args.at(0))
            {
                std::vector<Client *> clients = (*it)->getClients();
                for (std::vector<Client *>::iterator it2 = clients.begin(); it2 != clients.end(); it2++)
                {
                    if ((*it2)->getNickname() == *it0)
                    {
                        if (server->checkClientPrivilege(client, *it) == false)
                        {
                            client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), args.at(0)));
                            return;
                        }
                        (*it)->removeClient(*it2, server);
                        client->reply("kicked " + *it0 + " from channel " + args.at(0));
                        goto label;
                    }
                    else if ((*it2) == clients.back() && (*it2)->getNickname() != *it0)
                    {
                        client->reply(ERR_USERNOTINCHANNEL(client->getNickname(), *it0, args.at(0)));
                        return;
                    }
                }
            }
            else
            {
                client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args.at(0)));
                return;
            }
        }
        label:;
    }
}

void Command::topic(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() < 1) // changeed
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
        return;
    }
    std::vector<Channel *> channels = server->getChannels();
    if (channels.size() == 0)
    {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args.at(0)));
        return;
    }
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if ((*it)->getName() == args.at(0))
        {
            if (!SearchUserInChannel(client, *it))
                return;
            if (args.size() == 1)
            {
                if ((*it)->getTopic() == "")
                {
                    (*it)->setTopicTime(server->getStartTime());
                    client->reply(RPL_NOTOPIC(client->getNickname(), args.at(0)));
                    client->reply(RPL_TOPICWHOTIME(client->getUsername(), args.at(0), client->getNickname(), (*it)->getTopicTime()));
                    return;
                }
                else
                {
                    (*it)->setTopicTime(server->getStartTime());
                    client->reply(RPL_TOPIC(client->getNickname(), args.at(0), (*it)->getTopic()));
                    client->reply(RPL_TOPICWHOTIME(client->getUsername(), args.at(0), client->getNickname(), (*it)->getTopicTime()));
                    return;
                }
            }
            else
            {
                if (!server->checkClientPrivilege(client, *it) && (*it)->getMode().find('t') != std::string::npos)
                {
                    client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), args.at(0)));
                    return;
                }
                if (args.at(1) == ":")
                {
                    // if ((*it)->getMode().find('t') != std::string::npos)
                    // {
                    //     client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), args.at(0)));
                    //     return;
                    // }
                    (*it)->setTopic("");
                    (*it)->setTopicTime(server->getStartTime());
                    client->reply(RPL_NOTOPIC(client->getNickname(), args.at(0)));
                    client->reply(RPL_TOPICWHOTIME(client->getUsername(), args.at(0), client->getNickname(), (*it)->getTopicTime()));
                    server->sendToAllClientsInChannel(TOPIC_MSG(client->getNickname(), client->getUsername(), client->getHostname(), args.at(0)," :No topic is set"), *it, client);
                }
                else if (args.at(1) != "" && args.at(1).at(0) != ':')
                {
                    std::string topic = "";
                    std::string channelName = args.at(0);
                    args.erase(args.begin());
                    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
                        topic.append(*it + " ");
                    (*it)->setTopic(topic);
                    (*it)->setTopicTime(server->getStartTime());
                    client->reply(RPL_TOPIC(client->getNickname(), (*it)->getName(), (*it)->getTopic()));
                    client->reply(RPL_TOPICWHOTIME(client->getUsername(), (*it)->getName(), client->getNickname(), (*it)->getTopicTime()));
                    server->sendToAllClientsInChannel(TOPIC_MSG(client->getNickname(), client->getUsername(), client->getHostname(), (*it)->getName(), (*it)->getTopic()), *it, client);
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
    if (args.at(0).at(0) == '#')
    {
        std::vector<Channel *> channels = server->getChannels();
        for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            if ((*it)->getName() == args.at(0))
            {
                if ((*it)->CheckClientIsOperator(client->getNickname()) == false)
                {
                    client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), args.at(0)));
                    return;
                }
                if (!SearchUserInChannel(client, *it))
                    continue;
                if (args.size() == 1)
                {
                    client->reply(RPL_CHANNELMODEIS(client->getNickname(), args.at(0), (*it)->getMode(), ' '));
                    return;
                }
                int flag = 1;
                for (int i = 0; args[1][i] ; i++)
                {
                    if (args.at(1).at(i) == '+')
                        flag = 1;
                    else if (args.at(1).at(i) == '-')
                        flag = 2;
                    else if (args.at(1).at(i) == 'i')
                    {
                        if (flag == 1 && (*it)->getMode().find('i') == std::string::npos)
                            SetModeAndSandMessage(client, args.at(0), it, "i", 1);
                        else if (flag == 2 && (*it)->getMode().find('i') != std::string::npos)
                            SetModeAndSandMessage(client, args.at(0), it, "i", 2);
                    }
                    else if (args.at(1).at(i) == 't')
                    {
                        if (flag == 1 && (*it)->getMode().find('t') == std::string::npos)
                        {
                            (*it)->setTopicTime(std::to_string(time(0)));
                            (*it)->setTopic("");
                            SetModeAndSandMessage(client, args.at(0), it, "t", 1);
                        }
                        else if (flag == 2 && (*it)->getMode().find('t') != std::string::npos)
                        {
                            (*it)->setTopicTime(std::to_string(time(0)));
                            (*it)->setTopic("Default topic");
                            SetModeAndSandMessage(client, args.at(0), it, "t", 2);
                        }
                    }
                    else if (args.at(1).at(i) == 'k')
                    {
                        if (args.size() == 2 && flag == 1)
                        {
                            client->reply(ERR_INVALIDMODEPARAM(client->getNickname(), args.at(0), args.at(1).at(i), "<key>"));
                            if (!args.at(1)[i + 1])
                                return;
                            continue;
                            // i++;
                            // goto label;
                        }
                        else if (flag == 1 && (*it)->getMode().find('k') == std::string::npos)
                        {
                            SetModeAndSandMessage(client, args.at(0), it, "k", 1);
                            (*it)->setChannelKey(args.at(2));
                        }
                        else if (flag == 2 && (*it)->getMode().find('k') != std::string::npos)
                        {
                            SetModeAndSandMessage(client, args.at(0), it, "k", 2);
                            (*it)->setChannelKey("");
                            return;
                        }
                    }
                    else if (args.at(1).at(i) == 'o')
                    {
                        if (flag == 1 && (*it)->CheckClientIsOperator(client->getNickname()))
                        {
                            if (args.size() == 2)
                            {
                                client->reply(ERR_INVALIDMODEPARAM(client->getNickname(), args.at(0), args.at(1).at(i), "<nickname>"));
                                return;
                            }
                            if (check_if_user_is_in_channel(client, (*it)->getName(), channels) == false)
                            {
                                client->reply(ERR_NOSUCHNICK(client->getNickname(), args.at(2)));
                                return;
                            }
                            (*it)->AddChannelOperator((*it)->getClient(args.at(2)));
                            client->reply(MODE_MSG(client->getNickname(), client->getUsername(), client->getHostname(), args.at(0), "+o ", args.at(2)));
                            // client->reply(RPL_CHANNELMODEIS(client->getNickname(), args.at(0), (*it)->getMode(), "+o " + args.at(2)));
                        }
                        else if (flag == 2 &&(*it)->CheckClientIsOperator(client->getNickname()))
                        {
                            (*it)->RemoveChannelOperator((*it)->getClient(args.at(2)));
                            client->reply(MODE_MSG(client->getNickname(), client->getUsername(), client->getHostname(), args.at(0), "-o ", args.at(2)));
                            // client->reply(RPL_CHANNELMODEIS(client->getNickname(), args.at(0), (*it)->getMode(), "-o " + args.at(2)));
                        }
                    }
                    else if (args.at(1).at(i) == 'l')
                    {
                        if (args.size() == 2 && flag == 1)
                        {
                            client->reply(ERR_INVALIDMODEPARAM(client->getNickname(), args.at(0), args.at(1).at(i), "<limit>"));
                            if (!args.at(1)[i + 1])
                                return;
                            continue;
                            // i++;
                            // goto label;
                        }
                        if (flag == 1 && (*it)->getMode().find('l') == std::string::npos)
                        {
                            if (std::stoi(args.at(2)) <= 0 || std::stoi(args.at(2)) < (int)(*it)->getClients().size())
                            {
                                client->reply(ERR_INVALIDMODEPARAM(client->getNickname(), args.at(0), args.at(1).at(i), "<limit> (limit must be > 0 and > number of clients in channel)"));
                                return;
                            }
                            (*it)->setChannelLimit(std::stoi(args.at(2)));
                            (*it)->setChannelLimit(std::stoi(args.at(2)));
                            SetModeAndSandMessage(client, args.at(0), it, "l", 1);
                        }
                        else if (flag == 2 && (*it)->getMode().find('l') != std::string::npos)
                        {
                            (*it)->setChannelLimit(100);
                            SetModeAndSandMessage(client, args.at(0), it, "l", 2);
                        }    
                    }
                    else if (args.at(1).at(i) != 's' || args.at(1).at(i) != 'p')
                        client->reply(ERR_UNKNOWNMODE(client->getNickname(), args.at(1).at(i)));
                }
            }
            else if ((*it) == channels.back() && (*it)->getName() != args.at(0))
            {
                client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args.at(0)));
                return;
            }
        }
    }
    else
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args.at(0)));
}

void Command::join(Client *client, std::vector<std::string> args, Server *server)
{
   if (args.size() == 0)
   {
       client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), client->getCommand()));
       return;
   }
    std::vector<std::string> channel_names = client->split(args.at(0), ',');
    std::vector<std::string> keys;
    if (args.size() == 2)
        keys = client->split(args.at(1), ',');
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
            client->reply(ERR_BADCHANMASK(*it));
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
            new_channel->AddChannelOperator(client);
            if (keys.at(n).size() > 0)
            {
                new_channel->setMode(new_channel->getMode() + "k");
                new_channel->setChannelKey(keys.at(n));
                client->reply(JOIN_SUCC(client->getNickname(), client->getUsername(), client->getHostname(), *it));
                client->reply(RPL_NOTOPIC(client->getNickname(), *it));
                client->reply(RPL_NAMREPLY(client->getNickname(), *it, new_channel->getChannelUsersList(new_channel)));
                client->reply(RPL_ENDOFNAMES(client->getNickname(), *it));
            }
            else if (keys.at(n).size() == 0)
            {
                client->reply(JOIN_SUCC(client->getNickname(), client->getUsername(), client->getHostname(), *it));
                client->reply(RPL_NOTOPIC(client->getNickname(), *it));
                client->reply(RPL_NAMREPLY(client->getNickname(), *it, new_channel->getChannelUsersList(new_channel)));
                client->reply(RPL_ENDOFNAMES(client->getNickname(), *it));
            }
            if (keys.at(n) == keys.back())
            {
                keys.at(0) = "";
                n = 0;
            }
            else if (keys.at(n).size() > 0)
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
            if (client->getIsInvited())
            {
                if (exist_channel->getMode().find('k') != std::string::npos)
                    goto label;
                exist_channel->addClient(client);
                client->reply(JOIN_SUCC(client->getNickname(), client->getUsername(), client->getHostname(), exist_channel->getName()));
                server->sendToAllClientsInChannel(JOIN_SUCC(client->getNickname(), client->getUsername(), client->getHostname(), exist_channel->getName()), exist_channel,client);
                client->reply(RPL_TOPIC(client->getNickname(), *it, exist_channel->getTopic()));
                client->reply(RPL_TOPICWHOTIME(client->getNickname(), *it, exist_channel->getTopic(), exist_channel->getTopicTime()));
                client->reply(RPL_NAMREPLY(client->getNickname(), *it, exist_channel->getChannelUsersList(exist_channel)));
                client->reply(RPL_ENDOFNAMES(client->getNickname(), *it));
                continue;
            }
            client->reply(ERR_INVITEONLYCHAN(client->getNickname(), *it));
            continue;
        }
        else if (exist_channel->getMode().find('k') != std::string::npos)
        {
            label:
            if (keys.at(n) != exist_channel->getChannelKey())
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
            client->reply(JOIN_SUCC(client->getNickname(), client->getUsername(), client->getHostname(), exist_channel->getName()));
            server->sendToAllClientsInChannel(JOIN_SUCC(client->getNickname(), client->getUsername(), client->getHostname(), exist_channel->getName()), exist_channel,client);
            client->reply(RPL_TOPIC(client->getNickname(), *it, exist_channel->getTopic()));
            client->reply(RPL_TOPICWHOTIME(client->getNickname(), *it, exist_channel->getTopic(), exist_channel->getTopicTime()));
            client->reply(RPL_NAMREPLY(client->getNickname(), *it, exist_channel->getChannelUsersList(exist_channel)));
            client->reply(RPL_ENDOFNAMES(client->getNickname(), *it));
        }
        else
        {
            exist_channel->addClient(client);
            client->reply(JOIN_SUCC(client->getNickname(), client->getUsername(), client->getHostname(), exist_channel->getName()));
            server->sendToAllClientsInChannel(JOIN_SUCC(client->getNickname(), client->getUsername(), client->getHostname(), exist_channel->getName()), exist_channel,client);
            client->reply(RPL_NOTOPIC(client->getNickname(), *it));
            client->reply(RPL_NAMREPLY(client->getNickname(), *it, exist_channel->getChannelUsersList(exist_channel)));
            client->reply(RPL_ENDOFNAMES(client->getNickname(), *it));
        }
    }
}

void Command::part(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));
        return;
    }
    std::string msg;
    if (args.size() >= 1)
        for (std::vector<std::string>::iterator it = args.begin() + 1; it != args.end(); it++)
            msg.append(*it + " ");
    std::vector<std::string> channelsToPart = client->split(args.at(0), ',');
    for (std::vector<std::string>::iterator it = channelsToPart.begin(); it != channelsToPart.end(); it++)
    {
        std::vector<Channel *> channels = server->getChannels();
        for (std::vector<Channel *>::iterator it2 = channels.begin(); it2 != channels.end(); it2++)
        {
            if ((*it2)->getName() == *it)
            {
                if (!SearchUserInChannel(client, *it2))
                    continue;
                (*it2)->removeClient(client, server);
                    // server->sendToAllClientsInChannel(MODE_MSG(client->getNickname(), client->getUsername(), client->getHostname(), *it, "-o ", client->getNickname()), *it2, client);
                if (msg != "")
                    client->reply(msg);
                server->sendToAllClientsInChannel(PART_MSG(client->getNickname(), client->getUsername(), client->getHostname(), *it, msg), *it2, client);
                break;
            }
            else if ((*it2) == channels.back() && (*it2)->getName() != *it)
                client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), *it));
        }
    }
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
    std::vector<std::string> channels = client->split(args.at(0), ',');
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        for (std::vector<Channel *>::iterator it2 = serverChannels.begin(); it2 != serverChannels.end(); it2++)
        {
            if ((*it2)->getName() == *it)
            {
                std::vector<Client *> clients = (*it2)->getClients();
                for (std::vector<Client *>::iterator it3 = clients.begin(); it3 != clients.end(); it3++)
                {
                    client->reply(RPL_NAMREPLY(client->getNickname(), *it, (*it2)->getChannelUsersList(*it2)));
                }
                client->reply(RPL_ENDOFNAMES(client->getNickname(), *it));
                break;
            }
            else if ((*it2) == serverChannels.back() && (*it2)->getName() != *it)
            {
                client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), *it));
                break;
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
        if (args.at(1) == '>')
        {
            if (minutes >= std::stod(value))
            {
                std::string message = RPL_LIST(client->getNickname(), (*it)->getName(), size, (*it)->getTopic());
            }
        }
        else if (args.at(1) == '<')
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
        if (args.at(0).at(0) == '#')
        {
            conditions = client->split(args.at(0), ',');
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
            if (args.at(0).at(0) == 'C')
            {
                std::string value = args.at(0).substr(2);
                ListConditions(client, channels, value, args.at(0));
            }
            else if (args.at(0).at(0) == 'U')
            {
                std::string value = args.at(0).substr(2);
                if (value.find_first_not_of("0123456789") != std::string::npos || value.empty())
                {
                    client->reply("invalid condition" + args.at(0));
                    return;
                }
                for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
                {
                    if (args.at(0).at(1) == '>')
                    {
                        if ((*it)->getClients().size() >= std::stod(value))
                        {
                            std::string msg = (*it)->getName() + ": Connected clients : " + std::to_string((*it)->getClients().size());
                            client->reply(msg);
                        }
                    }
                    else if (args.at(0).at(1) == '<')
                    {
                        if ((*it)->getClients().size() <= std::stod(value))
                        {
                            std::string msg = (*it)->getName() + ": Connected clients : " + std::to_string((*it)->getClients().size());
                            client->reply(msg);
                        }
                    }
                }
            }
            else if (args.at(0).at(0) == '>' || args.at(0).at(0) == '<')
            {
                std::string value = args.at(0).substr(1);
                ListConditions(client, channels, value, "C" + args.at(0));
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
    std::string target = args.at(0);
    std::string msg;
    for (std::vector<std::string>::iterator it = args.begin() + 1; it != args.end(); it++)
        msg.append(*it + " ");
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
    msg = msg.at(0) == ':' ? msg.substr(1) : msg;
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
                    if (!check_if_user_is_in_channel(client, *it, channels))
                        continue;
                    std::vector<Client *> clients = (*it2)->getClients();
                    for (std::vector<Client *>::iterator it3 = clients.begin(); it3 != clients.end(); it3++)
                    {
                        if ((*it3)->getNickname() == client->getNickname())
                            continue;
                        (*it3)->reply(PRIVMSG(client->getNickname(), client->getUsername(), client->getHostname(), (*it2)->getName(), msg));
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
        std::vector<Client *> users = server->getUsers();
        for (std::vector<Client *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            if ((*it2)->getNickname() == *it)
            {
                if ((*it2)->getNickname() == client->getNickname())
                    continue;
                (*it2)->reply(PRIVMSG(client->getNickname(), client->getUsername(), client->getHostname(), (*it2)->getNickname(), msg));
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
        return;
    std::string target = args.at(0);
    std::string msg;
    for (std::vector<std::string>::iterator it = args.begin() + 1; it != args.end(); it++)
        msg.append(*it + " ");
    if (msg.empty() || target.empty())
        return;
    std::vector<std::string> targets = client->split(target, ',');
    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
    {
        if (it->find_first_not_of("#abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890[]{}\\|") != std::string::npos ||
                it->empty() || it->find_first_of("0123456789") == 0)
            continue;
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
                    break;
            }
            continue;
        }
        std::vector<Client *> users = server->getUsers();
        for (std::vector<Client *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            if ((*it2)->getNickname() == *it)
            {
                if ((*it2)->getNickname() == client->getNickname())
                    continue;
                (*it2)->reply(NOTICE(client->getNickname(), client->getUsername(), client->getHostname(), *it, msg));
                break;
            }
            else if ((*it2) == users.back() && (*it2)->getNickname() != *it)
                break;
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
                (*it)->removeClient(client, server);
                server->sendToAllClientsInChannel(QUIT_MSG(client->getNickname(), client->getUsername(), client->getHostname(), msg), *it, client);
            }
        }
    }
    client->reply(QUIT_MSG(client->getNickname(), client->getUsername(), client->getHostname(), msg));
    server->removeClient(client->getFd(), server);
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
        if ((*it)->getName() == args.at(0))
        {
            std::vector<Client *> clients = (*it)->getClients();
            for (std::vector<Client *>::iterator it2 = clients.begin(); it2 != clients.end(); it2++)
            {
                client->reply(RPL_WHOREPLY(client->getNickname(), args.at(0), (*it2)->getUsername(), (*it2)->getHostname(), server->getServerName(), (*it2)->getNickname(), (*it)->getMode(), (*it2)->getRealname()));
            }
            client->reply(RPL_ENDOFWHO(client->getNickname()));
            return;
        }
    }
    std::vector<Client *> users = server->getUsers();
    for (std::vector<Client *>::iterator it = users.begin(); it != users.end(); it++)
    {
        if ((*it)->getNickname() == args.at(0))
        {
            client->reply(RPL_WHOREPLY(client->getNickname(), args.at(0), (*it)->getUsername(), (*it)->getHostname(), server->getServerName(), (*it)->getNickname(), "", (*it)->getRealname()));
            client->reply(RPL_ENDOFWHO(client->getNickname()));
            return;
        }
    }
    client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), args.at(0)));
}

void Command::ping(Client *client, std::vector<std::string> args, Server *server)
{
    std::vector<Client *> users = server->getUsers();
    if (args.empty())
    {
        client->reply(ERR_NOORIGIN(client->getNickname()));
        return;
    }
    if (args.at(0) == "Problem_irc")
    {
        client->reply(PONG(client->getNickname(), args.at(0)));
        return;
    }
    for (std::vector<Client *> ::iterator it = users.begin(); it != users.end(); it++)
    {
        if ((*it)->getUsername() == args.at(0) || (*it)->getNickname() == args.at(0) || (*it)->getRealname() == args.at(0))
            (*it)->reply(PING(args.at(0)));
    }
}
void Command::pong(Client *client, std::vector<std::string> args, Server *server)
{
    std::vector<Client *> users = server->getUsers();
    if (args.empty())
    {
        client->reply(ERR_NOORIGIN(client->getNickname()));
        return;
    }
    for (std::vector<Client *> ::iterator it = users.begin(); it != users.end(); it++)
    {
        if ((*it)->getUsername() == args.at(0) || (*it)->getNickname() == args.at(0) || (*it)->getRealname() == args.at(0))
            (*it)->reply(PONG((*it)->getNickname(), args.at(0)));
    }
}

