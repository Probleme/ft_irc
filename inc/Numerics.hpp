/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 12:03:06 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/15 12:36:57 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_HPP
# define NUMERICS_HPP

#include "Client.hpp"


#define MY_HOSTNAME ":" + getHostname()

#define ERR_ALREADYREGISTERED(client) MY_HOSTNAME + " 462 " +  (client) + " " + " :You may not reregister"
#define ERR_ERRONEUSNICKNAME(client, command) MY_HOSTNAME + " 432 " +  (client) + " " + command + " ::Erroneus nickname"
#define ERR_COMMANDNOTFOUND(client, command) MY_HOSTNAME + " 421 " +  (client) + " " + (command) + " :Unknown command"
#define ERR_NOSUCHNICK(client1, client2) MY_HOSTNAME + " 401 " + (client1) + " " + (client2) + " :No such nick"
#define ERR_NEEDMOREPARAMS(client, command) MY_HOSTNAME +  " 461 " + (client) + " " + (command) + " :Not enough parameters"
#define ERR_NOSUCHCHANNEL(client, channel)  MY_HOSTNAME + " 403 " + (client) + " " + (channel) + " :No such channel"
#define ERR_BADCHANNELKEY(client, channel)   MY_HOSTNAME + " 475 " + (client) + " " + (channel) + " :Cannot join channel (+k)"
#define ERR_NOTONECHANNEL(client, channel)  MY_HOSTNAME + " 442 " + (client) + " " + (channel) + " :You're not on that channel"
#define ERR_CHANOPRIVSNEEDED(client, channel) MY_HOSTNAME +  " 482 " + (client) + " " + (channel) + " :You're not channel operator"
#define ERR_PASSWDMISTACH(client) MY_HOSTNAME + " 464 " + (client) + " :Password incorrect"
#define ERR_USERONCHANNEL(client, nick, channel) MY_HOSTNAME + " 443 " + (client) + " " + (nick) + " " + (channel) + " :is already on channel"
#define ERR_INVITEONLYCHAN(client, channel) MY_HOSTNAME + " 473 " + (client) + " " + (channel) + " :Cannot join channel (+i)"
#define ERR_BADCHANMASK(channel)  MY_HOSTNAME + " 476 " + (channel) + " :Bad Channel Mask"
#define ERR_UNKNOWNMODE(client, modechar) MY_HOSTNAME + " 472 " + (client) + " " + (modechar) + " :is unknown mode char to me"
#define ERR_TOOMANYCHANNELS(client, channel) MY_HOSTNAME +  " 405 " + (client) + " "  + (channel) + " :You have joined too many channels"
#define ERR_CHANNELISFULL(client, channel)  MY_HOSTNAME + " 471 " + (client) + " " + (channel) + " :Cannot join channel (+l)"
#define ERR_KEYALREADYSET(client, channel) MY_HOSTNAME + " 467 " + (client) + " " + (channel) + " :Channel key already set"
#define ERR_NORECIPENT(client, command) MY_HOSTNAME + " 411 " + (client) + " :No recipient given " + (command) + ""
#define ERR_NOGIVENNICK(client)  MY_HOSTNAME + " 431 " + (client) + " :No nickname given"
#define ERR_ONEOUS(client, wrongnick) MY_HOSTNAME  +" 432 " + (client) + " " + (wrongnick) + " :Erroneous Nickname"
#define ERR_NICKINUSE(client, usednick) MY_HOSTNAME  + " 433 " + (client) +  " ana :Nickname is already in use."
#define ERR_NONICKNAMEGIVEN(client) MY_HOSTNAME + " 431 " + (client) + " :No nickname given"
#define ERR_NOTEXTTOSEND(client) MY_HOSTNAME + " 412 " + (client) + " :No text to send"
#define ERROR_MSG(reason) MY_HOSTNAME + " " + (reason)

#define NICK_MSG(client, username,ipaddr, newnick)  ":" + (client) + "!~" + (username) + "@"+ (ipaddr) + " NICK :"+(newnick)
#define KICK_MSG(client, ipaddr, channel, kicked, reason) ":" + (client) + "!~sad@" + (ipaddr) + " KICK " + (channel) + " " + kicked + " " + reason
#define QUIT_MSG(nickname, username, ipaddr, reason) ":"+ (nickname) + "!~" + (username) + "@" + (ipaddr) + " QUIT :Client Quit" + (reason)
#define PRIVMSG(client, username, ipaddr, target, message) ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " PRIVMSG " + (target) + " :" + (message)
#define JOIN_SUCC(nickname, username, client_ip, channel) ":" + (nickname) + "!~" + (username) + "@" + (client_ip) + " JOIN " + (channel)
#define MODE_MSG(client, username,ipaddr, channel,added_mode, target) ":" + (client) + "!~" + (username) +"@" + (ipaddr) + " MODE " + (channel) + " " +(added_mode) +  target
#define PART_MSG(nickname, username, ipaddr, channel, reason) ":" + (nickname) + "!~" + (username) + "@" + ipaddr + " PART " + (channel) + " " + (reason)
#define NOTICE_MSG(client, username, ipaddr, target, message) ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " NOTICE " + (target) + " :" + (message)

#define RPL_NAMERPLY(client, channel, users_list) MY_HOSTNAME + " 353 " + (client) + " = " + (channel) + " :" + (users_list)
#define RPL_CREATIONTIME(client, channel, creation_time) MY_HOSTNAME + " 329 " + (client) + " " + (channel) + " " + (creation_time)
#define RPL_ENDOFNAMES(client, channel)  MY_HOSTNAME + " 366 " + (client) + " " + (channel) + " :End of /NAMES list"
#define RPL_ENDOFINVITELIST(client) MY_HOSTNAME + " 337 " + " :End of /INVITE list"
#define RPL_CHANNELMODEIS(client, channel, modestring) MY_HOSTNAME + " 324 " + (client) + " "+ (channel) + " " +(modestring) 
#define RPL_TOPIC(client, channel, topic) MY_HOSTNAME + " 332 " + (client) + " " + (channel) +  " :" + (topic)
#define RPL_NOTOPIC(client, channel)  MY_HOSTNAME + " 331 " + (client) + " " + (channel) + " :No topic is set"
#define RPL_INVITING(client, nick, channel)  MY_HOSTNAME + " 341 " + (client) + " " + (nick) + " " + (channel)
#define RPL_TOPICWHOTIME(client, channel, nick, setat) MY_HOSTNAME + " 333 " + (client) + " " + (channel) + " " + (nick) + " " + (setat)
#define RPL_WELCOME(client) MY_HOSTNAME +  " 001 " + (client) + " :Welcome to the IRC Network, " + (client)
#define RPL_YOURHOST(client, hostname) MY_HOSTNAME + " 002 " + (client) + " :Your host is " + (hostname)
#define RPL_CREATED(client) MY_HOSTNAME + " 003 " + (client) + " :This server was created Mon Dec 26 2023 at 11:33:05 EDT"
#define RPL_ISUPPORT(client, values)  MY_HOSTNAME + " 005 " + (client) + " " + (values) + " :are supported by this server"
#define RPL_MOTD(client, sentence) MY_HOSTNAME + " 372 " + (client) + " " + (sentence)



#endif