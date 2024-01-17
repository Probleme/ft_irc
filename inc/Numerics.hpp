/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aer-raou <aer-raou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 12:03:06 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/17 13:01:43 by aer-raou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_HPP
# define NUMERICS_HPP


#define ERR_ALREADYREGISTERED(client) ":irc.1337.ma 462 " +  (client) + " " + " :You may not reregister"
#define ERR_ERRONEUSNICKNAME(client, command) ":irc.1337.ma 432 " +  (client) + " " + command + " ::Erroneus nickname"
#define ERR_UNKNOWNCOMMAND(client, command) ":irc.1337.ma 421 " +  (client) + " " + (command) + " :Unknown command"
#define ERR_NOSUCHNICK(client1, client2) ":irc.1337.ma 401 " + (client1) + " " + (client2) + " :No such nick"
#define ERR_NEEDMOREPARAMS(client, command) ":irc.1337.ma 461 " + (client) + " " + (command) + " :Not enough parameters"
#define ERR_NOSUCHCHANNEL(client, channel)  ":irc.1337.ma 403 " + (client) + " " + (channel) + " :No such channel"
#define ERR_BADCHANNELKEY(client, channel)   ":irc.1337.ma 475 " + (client) + " " + (channel) + " :Cannot join channel (+k)"
#define ERR_NOTONCHANNEL(client, channel)  ":irc.1337.ma 442 " + (client) + " " + (channel) + " :You're not on that channel"
#define ERR_CHANOPRIVSNEEDED(client, channel) ":irc.1337.ma 482 " + (client) + " " + (channel) + " :You're not channel operator"
#define ERR_PASSWDMISMATCH(client) ":irc.1337.ma 464 " + (client) + " :Password incorrect"
#define ERR_USERONCHANNEL(client, nick, channel) ":irc.1337.ma 443 " + (client) + " " + (nick) + " " + (channel) + " :is already on channel"
#define ERR_INVITEONLYCHAN(client, channel) ":irc.1337.ma 473 " + (client) + " " + (channel) + " :Cannot join channel (+i)"
#define ERR_BADCHANMASK(channel)  ":irc.1337.ma 476 " + (channel) + " :Bad Channel Mask"
#define ERR_UNKNOWNMODE(client, modechar) ":irc.1337.ma 472 " + (client) + " " + (modechar) + " :is unknown mode char to me"
#define ERR_TOOMANYCHANNELS(client, channel) ":irc.1337.ma 405 " + (client) + " "  + (channel) + " :You have joined too many channels"
#define ERR_CHANNELISFULL(client, channel)  ":irc.1337.ma 471 " + (client) + " " + (channel) + " :Cannot join channel (+l)"
#define ERR_KEYALREADYSET(client, channel) ":irc.1337.ma 467 " + (client) + " " + (channel) + " :Channel key already set"
#define ERR_NORECIPENT(client, command) ":irc.1337.ma 411 " + (client) + " :No recipient given " + (command) + ""
#define ERR_ONEOUS(client, wrongnick) MY_HOSTNAME +" 432 " + (client) + " " + (wrongnick) + " :Erroneous Nickname"
#define ERR_NICKNAMEINUSE(client, usednick) ":irc.1337.ma 433 " + (client) +  " ana :Nickname is already in use."
#define ERR_NONICKNAMEGIVEN(client) ":irc.1337.ma 431 " + (client) + " :No nickname given"
#define ERR_NOTEXTTOSEND(client) ":irc.1337.ma 412 " + (client) + " :No text to send"
#define ERROR_MSG(reason) ":irc.1337.ma " + (reason)
#define ERR_NOTREGISTERED(client) ":irc.1337.ma 451 " + (client) + " :You have not registered"

#define NICK_MSG(client, username,ipaddr, newnick)  ":" + (client) + "!~" + (username) + "@"+ (ipaddr) + " NICK :"+(newnick)
#define KICK_MSG(client, ipaddr, channel, kicked, reason) ":" + (client) + "!~sad@" + (ipaddr) + " KICK " + (channel) + " " + kicked + " " + reason
#define QUIT_MSG(nickname, username, ipaddr, reason) ":"+ (nickname) + "!~" + (username) + "@" + (ipaddr) + " QUIT :Client Quit" + (reason)
#define PRIVMSG(client, username, ipaddr, target, message) ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " PRIVMSG " + (target) + " :" + (message)
#define JOIN_SUCC(nickname, username, client_ip, channel) ":" + (nickname) + "!~" + (username) + "@" + (client_ip) + " JOIN " + (channel)
#define MODE_MSG(client, username,ipaddr, channel,added_mode, target) ":" + (client) + "!~" + (username) +"@" + (ipaddr) + " MODE " + (channel) + " " +(added_mode) +  target
#define PART_MSG(nickname, username, ipaddr, channel, reason) ":" + (nickname) + "!~" + (username) + "@" + ipaddr + " PART " + (channel) + " " + (reason)
#define NOTICE_MSG(client, username, ipaddr, target, message) ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " NOTICE " + (target) + " :" + (message)

#define RPL_NAMERPLY(client, channel, users_list) ":irc.1337.ma 353 " + (client) + " = " + (channel) + " :" + (users_list)
#define RPL_CREATIONTIME(client, channel, creation_time) ":irc.1337.ma 329 " + (client) + " " + (channel) + " " + (creation_time)
#define RPL_ENDOFNAMES(client, channel)  ":irc.1337.ma 366 " + (client) + " " + (channel) + " :End of /NAMES list"
#define RPL_ENDOFINVITELIST(client) ":irc.1337.ma 337 " + " :End of /INVITE list"
#define RPL_CHANNELMODEIS(client, channel, modestring) ":irc.1337.ma 324 " + (client) + " "+ (channel) + " " +(modestring) 
#define RPL_TOPIC(client, channel, topic) ":irc.1337.ma 332 " + (client) + " " + (channel) +  " :" + (topic)
#define RPL_NOTOPIC(client, channel)  ":irc.1337.ma 331 " + (client) + " " + (channel) + " :No topic is set"
#define RPL_INVITING(client, nick, channel)  ":irc.1337.ma 341 " + (client) + " " + (nick) + " " + (channel)
#define RPL_TOPICWHOTIME(client, channel, nick, setat) ":irc.1337.ma 333 " + (client) + " " + (channel) + " " + (nick) + " " + (setat)
#define RPL_WELCOME(client) ":irc.1337.ma 001 " + (client) + " :Welcome to the IRC Network, " + (client)
#define RPL_YOURHOST(client, hostname) ":irc.1337.ma 002 " + (client) + " :Your host is " + (hostname)
#define RPL_CREATED(client) ":irc.1337.ma 003 " + (client) + " :This server was created Mon Dec 26 2023 at 11:33:05 EDT"
#define RPL_ISUPPORT(client, values)  ":irc.1337.ma 005 " + (client) + " " + (values) + " :are supported by this server"
#define RPL_MOTD(client, sentence) ":irc.1337.ma 372 " + (client) + " " + (sentence)
#define RPL_ENDOFWHO(client, channel) ":irc.1337.ma 315 " + (client) + " " + (channel) + " :End of /WHO list"
#define RPL_LISTSTART(client) ":irc.1337.ma 321 " + (client) + " Channel :Users  Name"
#define RPL_LIST(client, channel, nb_users, topic) ":irc.1337.ma 322 " + (client) + " " + (channel) + " " + (nb_users) + " :" + (topic)

#endif