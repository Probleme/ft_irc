/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aer-raou <aer-raou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 08:44:25 by aer-raou          #+#    #+#             */
/*   Updated: 2024/01/22 11:59:21 by aer-raou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Command.hpp"
#include "../inc/randomMsg.hpp"

std::string RANDOM_MSG(std::string nickname)
{
    std::string message;
    std::string msg = "RANDOM_MSG_" + nickname;
    int i = rand() % 20;
    switch (i)
    {
    case 0:
        message = RANDOM_MSG0(nickname);
        break;
    case 1:
        message = RANDOM_MSG1(nickname);
        break;
    case 2:
        message = RANDOM_MSG2(nickname);
        break;
    case 3:
        message = RANDOM_MSG3(nickname);
        break;
    case 4:
        message = RANDOM_MSG4(nickname);
        break;
    case 5:
        message = RANDOM_MSG5(nickname);
        break;
    case 6:
        message = RANDOM_MSG6(nickname);
        break;
    case 7:
        message = RANDOM_MSG7(nickname);
        break;
    case 8:
        message = RANDOM_MSG8(nickname);
        break;
    case 9:
        message = RANDOM_MSG9(nickname);
        break;
    case 10:
        message = RANDOM_MSG10(nickname);
        break;
    case 11:
        message = RANDOM_MSG11(nickname);
        break;
    case 12:
        message = RANDOM_MSG12(nickname);
        break;
    case 13:
        message = RANDOM_MSG13(nickname);
        break;
    case 14:
        message = RANDOM_MSG14(nickname);
        break;
    case 15:
        message = RANDOM_MSG15(nickname);
        break;
    case 16:
        message = RANDOM_MSG16(nickname);
        break;
    case 17:
        message = RANDOM_MSG17(nickname);
        break;
    case 18:
        message = RANDOM_MSG18(nickname);
        break;
    case 19:
        message = RANDOM_MSG19(nickname);
        break;
    default:
        message = RANDOM_MSG20(nickname);
        break;
    }
    return (message);
}

void Command::bot(Client *client, std::vector<std::string> args, Server *server)
{
    if (args.size() > 0)
    {
        if (args.at(0) == "logtime")
        {
            Client *target = server->getClientByNickname(args.at(1));
            if (target)
            {
                int minutes = (time(0) - target->getTime()) / 60;
                int seconds = (time(0) - target->getTime()) % 60;
                std::string message = target->getNickname() + " is loged since " + std::to_string(minutes) + " minutes and " + std::to_string(seconds) + " seconds";
                server->sendReplyToClient(client, message);
            }
            else
            {
                int minutes = (time(0) - client->getTime()) / 60;
                int seconds = (time(0) - client->getTime()) % 60;
                std::string message = client->getNickname() + " is loged since " + std::to_string(minutes) + " minutes and " + std::to_string(seconds) + " seconds";
                server->sendReplyToClient(client, message);
            }
        }
        else if (args.at(0) == "help")
        {
            std::string message = "Available commands for bot : \n";
            message += "bot help : print all commands available for bot and all commands available for client\n";
            message += "bot logtime <client> : return time since client is loged in minutes and seconds\n";
            message += "bot random_msg <client> : send a random message to a client\n";
            message += "random_msg example: \n";
            message += "\tSalutations! I'm your trusty bot companion. <nickname> has a message just for you.\n";
            message += "\tHey! I'm the talkative bot. <nickname> has a message waiting for you..\n";
            message += "\tGreetings, human! I'm here to assist. <nickname> is ready for a chat with you.\n";
            message += "bot is_logged <client> : check if a client is logged\n";
            message += "Available commands for client : \n";
            message += "NICK <nickname> : change your nickname\n";
            message += "USER <username> <hostname> <servername> <realname> : set your username, hostname, servername and realname\n";
            message += "JOIN <channel> : join a channel\n";
            message += "PART <channel> : leave a channel\n";
            message += "PRIVMSG <client> <message> : send a private message to a client\n";
            message += "NOTICE <client> <message> : send a notice to a client\n";
            message += "QUIT <message> : quit the server\n";
            message += "LIST : list all channels\n";
            message += "WHO : list all clients\n";
            message += "KICK <client> <channel> : kick a client from a channel\n";
            message += "MODE <channel> <mode> : set mode for a channel\n";
            message += "PING <server> : ping a server\n";
            message += "PONG <server> : pong a server\n";
            message += "PASS <password> : set password for your client\n";
            message += "NAMES <channel> : list all clients in a channel\n";
            message += "TOPIC <channel> <topic> : set topic for a channel\n";
            message += "INVITE <client> <channel> : invite a client to a channel\n";
            server->sendReplyToClient(client, message);
        }
        else if (args.at(0) == "random_msg")
        {
            Client *target = server->getClientByNickname(args.at(1));
            if (target)
            {
                std::string message = RANDOM_MSG(client->getNickname());
                server->sendReplyToClient(target, message);
            }
            else
            {
                std::string message = "No such nick, usage : bot random_msg <client>";
                server->sendReplyToClient(client, message);
            }
        }
        else if (args.at(0) == "is_logged")
        {
            if (args.size() == 1)
            {
                std::string message = "Bot : Wrong number of arguments, usage : bot is_logged <client>";
                server->sendReplyToClient(client, message);
                return; 
            }
            Client *target = server->getClientByNickname(args.at(1));
            if (target)
            {
                std::string message = target->getNickname() + " is logged";
                server->sendReplyToClient(client, message);
            }
            else
            {
                std::string message = args.at(1) + " is not logged";
                server->sendReplyToClient(client, message);
            }
        }
        else
        {
            std::string message = "Bot : Unknown command : " + args.at(0);
            server->sendReplyToClient(client, message);
        }
    }
    else
    {
        std::string message = "Bot : Wrong number of arguments, usage : bot <command> <args>";
        server->sendReplyToClient(client, message);
    }
}
