/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 12:17:01 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/17 16:01:21 by ataouaf          ###   ########.fr       */
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
        time_t _ChannelCreatedAt;
        std::string _mode;
        std::string _key;
        std::string _banmask;
        std::string _exceptionmask;
        std::string _invitemask;
        std::string _operator;
        int          _userlimit;
        int         _topic;
        int         _limit;


    public:
        Channel(std::string name, Client *client);
        ~Channel();
        std::string setName(std::string name);
        std::string const getName() const;
        std::vector<Client *> getClients();
        void addClient(Client *client);
        void removeClient(Client *client);
        time_t getChannelCreationTime() const;
        void setChannelCreationTime(time_t time);
        void setMode(std::string mode);
        void setTopic(int topic);
        void setChannelKey(std::string key);
        void setChannelLimit(int limit);
        void setBanMask(std::string banmask);
        void setExceptionMask(std::string exceptionmask);
        void setInviteMask(std::string invitemask);
        void setUserLimit(int userlimit);
        void setChannelOperator(std::string oper);
        std::string getMode() const;
        std::string getChannelKey() const;
        std::string getBanMask() const;
        std::string getExceptionMask() const;
        std::string getInviteMask() const;
        std::string getChannelOperator() const;
        int getTopic() const;
        int getUserLimit() const;
        int getChannelLimit() const;

};

#endif