/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aer-raou <aer-raou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 12:17:01 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/21 18:42:54 by aer-raou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CHANNEL_HPP
# define CHANNEL_HPP

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

class Channel
{
    private:
        std::string _name;
        std::vector<Client *> _clients;
        std::vector<Client *> _operator;
        time_t _ChannelCreatedAt;
        std::string _mode;
        std::string _key;
        std::string _banmask;
        std::string _exceptionmask;
        std::string  _topic;
        std::string _topicTime;
        int         _invitemask;
        int         _userlimit;
        int         _limit;


    public:
        Channel(std::string name, Client *client);
        ~Channel();
        std::string setName(std::string name);
        std::string const getName() const;
        std::vector<Client *> getClients();
        std::vector<Client *> getChannelOperators() const;
        void addClient(Client *client);
        void removeClient(Client *client);
        time_t getChannelCreationTime() const;
        void setChannelCreationTime(time_t time);
        void setMode(std::string mode);
        void setTopic(std::string  topic);
        void setChannelKey(std::string key);
        void setChannelLimit(int limit);
        void setBanMask(std::string banmask);
        void setExceptionMask(std::string exceptionmask);
        void setInviteMask(int invitemask);
        void setUserLimit(int userlimit);
        void AddChannelOperator(Client *oper);
        void RemoveChannelOperator(Client *oper);
        std::string getMode() const;
        std::string getChannelKey() const;
        std::string getBanMask() const;
        std::string getExceptionMask() const;
        int getInviteMask() const;
        std::string getTopic() const;
        int getUserLimit() const;
        int getChannelLimit() const;
        void setTopicTime(std::string time);
        std::string getTopicTime() const;
        bool CheckClientIsOperator(std::string nickname);
        Client *getClient(std::string nickname);
        void replyToAllUsersInChannel(std::string message, Client *client);
};

#endif




/*

NICk test
:irc.1337.ma 421 * NICk :Unknown command
NICK test
:irc.1337.ma 451 * :You have not registered
USER ataouaf test test test
:irc.1337.ma 451 * :You have not registered
PASS password
NICK test
:*!~@10.11.13.1 NICK :test
USER atapuad test est test test
:irc.1337.ma 001 test :Welcome to the IRC Network, test
:irc.1337.ma 002 test :Your host is Problem_irc, running version 1.0.0
:irc.1337.ma 003 test :This server was created Sun Jan 21 18:34:42 2024
:irc.1337.ma 004 test :Problem_irc 1.0.0
:irc.1337.ma 005 test :are supported by this server
NICk test
:irc.1337.ma 421 * NICk :Unknown command
NICK test
:irc.1337.ma 451 * :You have not registered
USER ataouaf test test test
:irc.1337.ma 451 * :You have not registered
PASS password
NICK test
:*!~@10.11.13.1 NICK :test
USER atapuad test est test test
:irc.1337.ma 001 test :Welcome to the IRC Network, test
:irc.1337.ma 002 test :Your host is Problem_irc, running version 1.0.0
:irc.1337.ma 003 test :This server was created Sun Jan 21 18:34:42 2024
:irc.1337.ma 004 test :Problem_irc 1.0.0
:irc.1337.ma 005 test :are supported by this server
JOIN #chan1,#chan2,#chan3
:irc.1337.ma 461 test JOIN :Not enough parameters
JOIN #chan1,#chan2,#chan3 pass1
:irc.1337.ma 331 test #chan1 :No topic is set
:irc.1337.ma 353 test = #chan1 :test
:irc.1337.ma 366 test = #chan1 :End of /NAMES list
:test!~atapuad@10.11.13.1 JOIN #chan1
:irc.1337.ma 331 test #chan2 :No topic is set
:irc.1337.ma 353 test = #chan2 :test
:irc.1337.ma 366 test = #chan2 :End of /NAMES list
:irc.1337.ma 331 test #chan3 :No topic is set
:irc.1337.ma 353 test = #chan3 :test
:irc.1337.ma 366 test = #chan3 :End of /NAMES list
:irc.1337.ma 421 test Ip�JOIN :Unknown command
:irc.1337.ma 461 test JOIN :Not enough parameters
JOIN #chan1,#chan2,#chan3 pass1
:irc.1337.ma 331 test #chan1 :No topic is set
:irc.1337.ma 353 test = #chan1 :test
:irc.1337.ma 366 test = #chan1 :End of /NAMES list
:test!~atapuad@10.11.13.1 JOIN #chan1
:irc.1337.ma 331 test #chan2 :No topic is set
:irc.1337.ma 353 test = #chan2 :test
:irc.1337.ma 366 test = #chan2 :End of /NAMES list
:irc.1337.ma 331 test #chan3 :No topic is set
:irc.1337.ma 353 test = #chan3 :test
:irc.1337.ma 366 test = #chan3 :End of /NAMES list
:irc.1337.ma 421 test Ip�JOIN :Unknown command

*/