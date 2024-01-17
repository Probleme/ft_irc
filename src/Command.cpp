/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 05:01:11 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/17 16:15:04 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"


Command::~Command()
{
}

Command::Command()
{
    _commands["NICK"] = &Command::nick;
    _commands["USER"] = &Command::user;
    _commands["JOIN"] = &Command::join;
    _commands["PART"] = &Command::part;
    _commands["PRIVMSG"] = &Command::privmsg;
    // _commands["INVITE"] = &Command::invite;
    _commands["QUIT"] = &Command::quit;
    _commands["LIST"] = &Command::list;
    _commands["WHO"] = &Command::who;
    _commands["KICK"] = &Command::kick;
    _commands["MODE"] = &Command::mode;
    _commands["PING"] = &Command::ping;
    _commands["PONG"] = &Command::pong;
    _commands["NOTICE"] = &Command::notice;
    _commands["PASS"] = &Command::pass;
    _commands["NAMES"] = &Command::names;
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
    std::vector<Client *> users = server->getUsers();
    if (args.size() == 0)
    {
        client->setMessage(ERR_NONICKNAMEGIVEN(client->getNickname()));
        client->sendMessage();
        return;
    }
    /*
        If the server does not accept the new nickname supplied by the client as valid (for instance, due to containing invalid characters),
        it should issue an ERR_ERRONEUSNICKNAME numeric and ignore the NICK command. Servers MUST allow at least all alphanumerical characters,
        square and curly brackets ([]{}), backslashes (\), and pipe (|) characters in nicknames, and MAY disallow digits as the first character.
        Servers MAY allow extra characters, as long as they do not introduce ambiguity in other commands
    */
    if (args[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890[]{}\\|") != std::string::npos ||
            args[0].empty() || args[0].find_first_of("0123456789") == 0)
    {
        client->setMessage(ERR_ERRONEUSNICKNAME(client->getNickname(), client->getCommand()));
        client->sendMessage();
        return;
    }
    /*  If the server receives a NICK command from a client where the desired nickname is already in use on the network,
        it should issue an ERR_NICKNAMEINUSE numeric and ignore the NICK command.
    */
    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
    {
        for (std::vector<Client *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            if ((*it2)->getNickname() == *it)
            {
                client->setMessage(ERR_NICKNAMEINUSE(client->getNickname()));
                client->sendMessage();
                return;
            }
        }
    }
    std::string old_nickname = client->getNickname();
    client->setNickname(args[0]);
    client->setMessage(RPL_NICK(old_nickname, client->getUsername(), client->getHostname(), client->getNickname()));
    client->sendMessage();
}

void Command::user(Client *client, std::vector<std::string> args, Server *server)
{
    (void)server;
    if (client->isRegistered() && client->getUsername() != "" && client->getRealname() != "")
    {
        client->setMessage(ERR_ALREADYREGISTERED(client->getNickname()));
        client->sendMessage();
        return;
    }
    if (args.size() < 4 || args[0][0] == '\0')
    {
        client->setMessage(ERR_NEEDMOREPARAMS(client->getNickname(), client->getCommand()));
        client->sendMessage();
        return;
    }
    // USER aer 0 * :aer raou Ahmed
    std::string realname;
    if (args.size() > 4 && args[3][0] == ':')
    {
        for (std::vector<std::string>::iterator it = args.begin() + 3; it != args.end(); it++)
            realname += *it + " ";
        client->setRealname(realname);
        return;
    }
    /*
        It must be noted that <realname> must be the last parameter because it may contain SPACE (' ', 0x20) characters,
        and should be prefixed with a colon (:) if required.
    */
    else if (args[3][0] != ':')
        realname = args[3];
    /*
        The minimum length of <username> is 1, ie. it MUST NOT be empty.
        If it is empty, the server SHOULD reject the command with ERR_NEEDMOREPARAMS (even if an empty parameter is provided);
         otherwise it MUST use a default value instead.
    */
    /*
        The second and third parameters of this command SHOULD be sent as one zero ('0', 0x30) and one asterisk character
        ('*', 0x2A) by the client, as the meaning of these two parameters varies between different versions of the IRC protocol.
    */
    // if (args[1] != "0" || args[2] != "*")
    // {
    //     client->reply("invalid arguments");
    //     client->reply("USER <username> <hostname>(0) <servername>(*) :<realname>");
    //     return;
    // }
    /*
        If a client tries to send the USER command after they have already completed registration with the server,
        the ERR_ALREADYREGISTERED reply should be sent and the attempt should fail.
    */
   // now i wan to check if the user is already registered or not
    client->setUsername(args[0]);
    client->setRealname(args[3]);
}

bool check_if_user_is_in_channel(Client *client, std::string channel_name, std::vector<Channel *> channels)
{
    // std::cout << "user name : " << client->getNickname() << std::endl;
    // std::cout << "user nick : " << client->getNickname() << std::endl;
    // std::cout << "user realname : " << client->getRealname() << std::endl;
    // std::cout << "user username : " << client->getUsername() << std::endl;
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        // std::cout << ": " << (*it)->getName() << std::endl;
        if ((*it)->getName() == channel_name)
        {
            std::vector<Client *> clients = (*it)->getClients();
            for (std::vector<Client *>::iterator it2 = clients.begin(); it2 != clients.end(); it2++)
            {
                if ((*it2)->getUsername() == client->getUsername())
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
            //check limit :irc.leet.com 471 ATA #13 :Cannot join channel (+l)
            if ((*it)->getClients().size() >= (unsigned long)(*it)->getChannelLimit())
            {
                client->setMessage(ERR_CHANNELISFULL(client->getNickname(), args[0]));
                client->sendMessage();
                return;
            }
            //:irc.leet.com 473 ATA #13 :Cannot join channel (+i)
            if ((*it)->getMode().find('i') != std::string::npos)
            {
                client->setMessage(ERR_INVITEONLYCHAN(client->getNickname(), args[0]));
                client->sendMessage();
                return;
            }
            //:irc.leet.com 475 ATA #13 :Cannot join channel (+k)
            if ((*it)->getMode().find('k') != std::string::npos)
            {
                client->setMessage(ERR_BADCHANNELKEY(client->getNickname(), args[0]));
                client->sendMessage();
                return;
            }
            (*it)->addClient(client);
            client->reply("Joined channel " + args[0]);
            return;
        }
    }
    Channel *channel = new Channel(args[0], client);
    server->addChannel(channel, client);
    client->reply("Created channel " + args[0]);
}

bool SearchUserInChannel(Client *client, Channel *channel)
{
    /*
        For each channel in the parameter of this command, if the channel exists and the client is not joined to it,
        they will receive an ERR_NOTONCHANNEL (442) reply and that channel will be ignored
          "<client> <channel> :You're not on that channel"
        Returned when a client tries to perform a channel-affecting command on a channel which the client isn’t a part of.
    */
    std::vector<Client *> clients = channel->getClients();
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if ((*it)->getUsername() == client->getUsername())
            return true;
    }
    client->reply(client->getUsername() + " " + channel->getName() + " :You're not on that channel " + channel->getName());

    return false;
}

void Command::part(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() < 1 || args.size() > 2)
    {
        client->setMessage(ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));
        client->sendMessage();
        return;
    }
    std::string msg;
    if (args.size() == 2)
        msg = args[1];
    else
        msg = "";
    std::vector<Channel *> channels = server->getChannels();
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if ((*it)->getName() == args[0])
        {
            if (!SearchUserInChannel(client, *it))
            {
                client->setMessage(ERR_NOTONCHANNEL(client->getNickname(), args[0]));
                client->sendMessage();
                return;
            }
            (*it)->removeClient(client);
            client->reply("left channel " + args[0]);
            if (msg != "")
                client->reply(msg);
            return;
        }
    }

    /*
        If the channel does not exist, the client will receive an ERR_NOSUCHCHANNEL (403) reply and that channel will be ignored.
          "<client> <channel> :No such channel"
        Indicates that no channel can be found for the supplied channel name.
        The text used in the last param of this message may vary.
    */
    client->setMessage(ERR_NOSUCHCHANNEL(client->getNickname(), args[0]));
    client->sendMessage();
}


/*
    The NAMES command is used to view the nicknames joined to a channel and their channel membership prefixes.
    The param of this command is a list of channel names, delimited by a comma (",", 0x2C) character.
    The channel names are evaluated one-by-one. For each channel that exists and they are able to see the users in,
    the server returns one of more RPL_NAMREPLY numerics containing the users joined to the channel and a single RPL_ENDOFNAMES numeric.
    If the channel name is invalid or the channel does not exist, one RPL_ENDOFNAMES numeric containing the given channel name should be returned.
    If the given channel has the secret channel mode set and the user is not joined to that channel, one RPL_ENDOFNAMES numeric is returned.
    Users with the invisible user mode set are not shown in channel responses unless the requesting client is also joined to that channel.
    Servers MAY allow more than one target channel. They can advertise the maximum the number of target users per NAMES command via the TARGMAX RPL_ISUPPORT parameter.
*/
void Command::names(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() == 0)
    {
        client->reply("invalid arguments");
        client->reply("NAMES <channel>");
        return;
    }
    std::vector<Channel *> serverChannels = server->getChannels();
    std::vector<std::string> channels = client->split(args[0], ',');

    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        bool found = false;
        for (std::vector<Channel *>::iterator it2 = serverChannels.begin(); it2 != serverChannels.end(); it2++)
        {
            if ((*it2)->getName() == *it)
                found = true;
        }
        if (!found)
        {
            // If the channel name is invalid or the channel does not exist, one RPL_ENDOFNAMES numeric containing the given channel name should be returned.
            client->setMessage(RPL_ENDOFNAMES(client->getNickname(), *it));
            client->sendMessage();
            return;
        }
    }
    /*
        !!!! If the given channel has the secret channel mode set and the user is not joined to that channel, one RPL_ENDOFNAMES numeric is returned.
        !!!! Users with the invisible user mode set are not shown in channel responses unless the requesting client is also joined to that channel.
    */
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        for (std::vector<Channel *>::iterator it2 = serverChannels.begin(); it2 != serverChannels.end(); it2++)
        {
            if ((*it2)->getName() == *it)
            {
                /*
                    The channel names are evaluated one-by-one. For each channel that exists and they are able to see the users in,
                    the server returns one of more RPL_NAMREPLY numerics containing the users joined to the channel and a single RPL_ENDOFNAMES numeric.
                    Users with the invisible user mode set are not shown in channel responses unless the requesting client is also joined to that channel.
                */
                std::vector<Client *> clients = (*it2)->getClients();
                for (std::vector<Client *>::iterator it3 = clients.begin(); it3 != clients.end(); it3++)
                {
                    client->setMessage(RPL_NAMERPLY(client->getNickname(), *it, (*it3)->getNickname()));
                    client->sendMessage();
                }
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
        if (args[1] == '>')
        {
            if (minutes >= std::stod(value))
            {
                std::string msg = (*it)->getName() + ": Connected clients : " + std::to_string((*it)->getClients().size());
                client->reply(msg);
            }
        }
        else if (args[1] == '<')
        {
            if (minutes <= std::stod(value))
            {
                std::string msg = (*it)->getName() + ": Connected clients : " + std::to_string((*it)->getClients().size());
                client->reply(msg);
            }
        }
    }
}
/*
    The first possible parameter to this command is a list of channel names, delimited by a comma (",", 0x2C) character.
    If this parameter is given, the information for only the given channels is returned. If this parameter is not given,
    the information about all visible channels (those not hidden by the secret channel mode rules) is returned.
    The second possible parameter to this command is a list of conditions as defined in the ELIST RPL_ISUPPORT parameter, delimited by a comma (",", 0x2C) character.
    Clients MUST NOT submit an ELIST condition unless the server has explicitly defined support for that condition with the ELIST token.
    If this parameter is supplied, the server filters the returned list of channels with the given conditions as specified in the ELIST documentation.
*/
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
    /*
        Format: ELIST=<string>
        The ELIST parameter indicates that the server supports search extensions to the LIST command.
        The value MUST be specified, and is a non-delimited list of letters, each of which denote an extension.
        The letters MUST be treated as being case-insensitive.
        The following search extensions are defined:
        C: Searching based on channel creation time, via the "C<val" and "C>val" modifiers to search for a channel that was created either less than val minutes ago, or more than val minutes ago, respectively
        M: Searching based on a mask.
        N: Searching based on a non-matching !mask. i.e., the opposite of M.
        T: Searching based on topic set time, via the "T<val" and "T>val" modifiers to search for a topic time that was set less than val minutes ago, or more than val minutes ago, respectively.
        U: Searching based on user count within the channel, via the "<val" and ">val" modifiers to search for a channel that has less or more than val users, respectively.
    */
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
                        client->setMessage((*it2)->getName() + ": Connected clients : " + std::to_string((*it2)->getClients().size()));
                        client->sendMessage();
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
                    client->reply("invalid arguments");
                    return;
                }
                if (value.find_first_not_of("0123456789") != std::string::npos)
                {
                    client->reply("invalid arguments");
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
    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
        msg += it->c_str();
    client->setMessage(msg);
    client->sendMessage();
    close(client->getFd());
}

void Command::who(Client *client, std::vector<std::string> args, Server *server)
{
    std::vector<Client *> users = server->getUsers();
    if (args.size() == 0)
    {
        for (std::vector<Client *>::iterator it = users.begin(); it != users.end(); it++)
        {
            client->setMessage(RPL_ENDOFWHO(client->getNickname())); // mustt be print args in the second argument in RPL_ENDOFWHO
            client->sendMessage();
        }
    }
    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
    {
        for (std::vector<Client *>::iterator it2 = users.begin(); it2 != users.end(); it2++)
        {
            if ((*it2)->getUsername() == *it || (*it2)->getNickname() == *it || (*it2)->getRealname() == *it)
            {
                client->setMessage(RPL_ENDOFWHO(client->getNickname())); // mustt be print args in the second argument in RPL_ENDOFWHO
                client->sendMessage();
            }
        }
    }
}

void Command::kick(Client *client, std::vector<std::string> args, Server *server)
{
    (void)client;
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

void Command::ping(Client *client, std::vector<std::string> args, Server *server)
{
    std::vector<Client *> users = server->getUsers();
    if (args.size() != 1)
    {
        /*
            ERR_NEEDMOREPARAMS (461)
                "<client> <command> :Not enough parameters"
            Returned when a client command cannot be parsed because not enough parameters were supplied.
            The text used in the last param of this message may vary.
            ERR_NOORIGIN (409)
                 "<client> :No origin specified"
            Indicates a PING or PONG message missing the originator parameter which is required by old IRC servers.
            Nowadays, this may be used by some servers when the PING <token> is empty.
        */
        client->setMessage(ERR_NEEDMOREPARAMS(client->getNickname(), "PING"));
        client->sendMessage();
        return;
    }
    if (args[0] == "Problem_irc")
    {
        client->reply("PONG Problem_irc");
        return;
    }
    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
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
        client->setMessage(ERR_NEEDMOREPARAMS(client->getNickname(), "PONG"));
        client->sendMessage();
        return;
    }
    if (args[0] == "Problem_irc")
    {
        client->reply("PING Problem_irc");
        return;
    }
    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
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
    if (args.size() != 1)
    {
        client->setMessage(ERR_NEEDMOREPARAMS(client->getNickname(), client->getCommand()));
        client->sendMessage();
        return;
    }
    if (args[0] != server->getPassword())
    {
        client->setMessage(ERR_PASSWDMISMATCH(client->getNickname()));
        client->sendMessage();
        return;
    }
    if (client->getPassword() == true)
    {
        client->setMessage(ERR_ALREADYREGISTERED(client->getNickname()));
        client->sendMessage();
        return;
    }
    client->setPassword(true);
    client->reply("Password accepted you have registered");
}

void Command::mode(Client *client, std::vector<std::string> args, Server *server)
{
    /*
        The MODE command is used to set or remove options (or modes) from a given target.
User mode
If <target> is a nickname that does not exist on the network, the ERR_NOSUCHNICK (401) numeric is returned.
If <target> is a different nick than the user who sent the command, the ERR_USERSDONTMATCH (502) numeric is returned.
If <modestring> is not given, the RPL_UMODEIS (221) numeric is sent back containing the current modes of the target user.
If <modestring> is given, the supplied modes will be applied, and a MODE message will be sent to the user containing the changed modes.
If one or more modes sent are not implemented on the server, the server MUST apply the modes that are implemented,
and then send the ERR_UMODEUNKNOWNFLAG (501) in reply along with the MODE message.

Channel mode
If <target> is a channel that does not exist on the network, the ERR_NOSUCHCHANNEL (403) numeric is returned.
If <modestring> is not given, the RPL_CHANNELMODEIS (324) numeric is returned. Servers MAY choose to hide sensitive information such as channel keys when sending the current modes.
Servers MAY also return the RPL_CREATIONTIME (329) numeric following RPL_CHANNELMODEIS.
If <modestring> is given, the user sending the command MUST have appropriate channel privileges on the target channel to change the modes given.
If a user does not have appropriate privileges to change modes on the target channel, the server MUST NOT process the message, and ERR_CHANOPRIVSNEEDED (482) numeric is returned.
If the user has permission to change modes on the target, the supplied modes will be applied based on the type of the mode (see below).
For type A, B, and C modes, arguments will be sequentially obtained from <mode arguments>. If a type B or C mode does not have a parameter when being set, the server MUST ignore that mode.
If a type A mode has been sent without an argument, the contents of the list MUST be sent to the user, unless it contains sensitive information the user is not allowed to access.
When the server is done processing the modes, a MODE command is sent to all members of the channel containing the mode changes. Servers MAY choose to hide sensitive information when sending the mode changes.
    */

   /*
        MODE - Change the channel’s mode:
        · i: Set/remove Invite-only channel
        · t: Set/remove the restrictions of the TOPIC command to channel operators
        · k: Set/remove the channel key (password)
        · o: Give/take channel operator privilege
        l: Set/remove the user limit to channel
        i: This mode is used to set or remove the Invite-only status of a channel. When this mode is set, only invited users can join the channel.
        t: This mode is used to set or remove the restrictions of the TOPIC command to channel operators. When this mode is set, only channel operators can change the topic of the channel.
        k: This mode is used to set or remove the channel key, which is essentially a password for the channel. When this mode is set, users need to provide the correct key to join the channel.
        o: This mode is used to give or take channel operator privileges. When this mode is set, a user can be given or stripped of operator privileges in the channel.
        l: This mode is used to set or remove the user limit to the channel. When this mode is set, a limit can be placed on the number of users that can join the channel.
   */
   if (args.size() == 0)
   {
        client->setMessage(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
       return;
   }
    if (args[0][0] == '#')
    {
        std::vector<Channel *> channels = server->getChannels();
        for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            if ((*it)->getName() == args[0])
            {
                std::cout<< "Channel mode" + (*it)->getMode() << std::endl;
                if (!SearchUserInChannel(client, *it))
                {
                    client->setMessage(ERR_NOTONCHANNEL(client->getNickname(), args[0]));
                    return;
                }
                if (args.size() == 1)
                {
                    client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode()));
                    client->sendMessage();
                    return;
                }
                if (args.size() == 2)
                {
                    int flag = 1;
                    if (args[1][0] == '-' || args[1][0] == '+')
                    {
                        if (args[1][0] == '-')
                            flag = 0;
                        args[1] = args[1].substr(1);
                    }
                    if (args[1].find_first_not_of("itklo") != std::string::npos)
                    {
                        client->setMessage(ERR_UNKNOWNMODE(client->getNickname(), args[1]));
                        client->sendMessage();
                        return;
                    }
                    if (args[1].find('i') != std::string::npos)
                    {
                    std::cout << "args[1] : " << args[1] << std::endl;
                        if (flag)
                        {
                            if ((*it)->getMode().find('i') == std::string::npos)
                            {
                                (*it)->setMode((*it)->getMode() + "i");
                                client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode()));
                                client->sendMessage();
                                return;
                            }
                            else
                                return;
                        }
                        else
                        {
                            if ((*it)->getMode().find('i') != std::string::npos)
                            {
                                (*it)->setMode((*it)->getMode().erase((*it)->getMode().find('i'), 1));
                                client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode()));
                                client->sendMessage();
                                return;
                            }
                            else
                                return;
                        }
                    }
                    else if (args[1].find('t') != std::string::npos)
                    {
                        if (flag)
                        {
                            if ((*it)->getMode().find('t') == std::string::npos)
                            {
                                (*it)->setMode((*it)->getMode() + "t");
                                client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode()));
                                client->sendMessage();
                                return;
                            }
                            else
                                return;
                        }
                        else
                        {
                            if ((*it)->getMode().find('t') != std::string::npos)
                            {
                                (*it)->setMode((*it)->getMode().erase((*it)->getMode().find('t'), 1));
                                client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode()));
                                client->sendMessage();
                                return;
                            }
                            else
                                return;
                        }
                    }
                    else if (args[1].find('k') != std::string::npos)
                    {
                        if (flag)
                        {
                            if ((*it)->getMode().find('k') == std::string::npos)
                            {
                                (*it)->setMode((*it)->getMode() + "k");
                                client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode()));
                                client->sendMessage();
                                return;
                            }
                            else
                                return;
                        }
                        else
                        {
                            if ((*it)->getMode().find('k') != std::string::npos)
                            {
                                (*it)->setMode((*it)->getMode().erase((*it)->getMode().find('k'), 1));
                                client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode()));
                                client->sendMessage();
                                return;
                            }
                            else
                                return;
                        }
                    }
                    else if (args[1].find('o') != std::string::npos)
                    {
                        if (flag)
                        {
                            if ((*it)->getMode().find('o') == std::string::npos)
                            {
                                (*it)->setMode((*it)->getMode() + "o");
                                client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode()));
                                client->sendMessage();
                                return;
                            }
                            else
                                return;
                        }
                        else
                        {
                            if ((*it)->getMode().find('o') != std::string::npos)
                            {
                                (*it)->setMode((*it)->getMode().erase((*it)->getMode().find('o'), 1));
                                client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode()));
                                client->sendMessage();
                                return;
                            }
                            else
                                return;
                        }
                    }
                    else if (args[1].find('l') != std::string::npos)
                    {
                        if (flag)
                        {
                            if ((*it)->getMode().find('l') == std::string::npos)
                            {
                                (*it)->setMode((*it)->getMode() + "l");
                                client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode()));
                                client->sendMessage();
                                return;
                            }
                            else
                                return;
                        }
                        else
                        {
                            if ((*it)->getMode().find('l') != std::string::npos)
                            {
                                (*it)->setMode((*it)->getMode().erase((*it)->getMode().find('l'), 1));
                                client->setMessage(RPL_CHANNELMODEIS(client->getNickname(), args[0], (*it)->getMode()));
                                client->sendMessage();
                                return;
                            }
                            else
                                return;
                        }
                    }
                }
            }
        }
    }
    else
    {
        client->setMessage(ERR_NOSUCHCHANNEL(client->getNickname(), args[0]));
        client->sendMessage();
    }
                     
}
