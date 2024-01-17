/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aer-raou <aer-raou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:27:25 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/17 12:26:03 by aer-raou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/Channel.hpp"

Channel::Channel(std::string name, Client *client)
{
    this->_name = name;
    this->_clients.push_back(client);
    this->_ChannelCreatedAt = time(0);
    this->_mode = "";
    this->_topic = 0;
    this->_key = "";
    this->_banmask = "";
    this->_exceptionmask = "";
    this->_invitemask = "";
    this->_userlimit = 0;
    this->_operator = "";
    this->_limit = 0;
}

Channel::~Channel()
{
}

std::string Channel::setName(std::string name)
{
    this->_name = name;
    return (this->_name);
}

std::string const Channel::getName() const
{
    return (this->_name);
}

std::vector<Client *> Channel::getClients()
{
    return (this->_clients);
}

void Channel::addClient(Client *client)
{
    this->_clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
    for (size_t i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i] == client)
        {
            this->_clients.erase(this->_clients.begin() + i);
            return;
        }
    }
}

time_t Channel::getChannelCreationTime() const
{
    return (this->_ChannelCreatedAt);
}

void Channel::setChannelCreationTime(time_t time)
{
    this->_ChannelCreatedAt = time;
}

void Channel::setMode(std::string mode)
{
    this->_mode = mode;
}

void Channel::setTopic(int topic)
{
    this->_topic = topic;
}

void Channel::setChannelKey(std::string key)
{
    this->_key = key;
}

void Channel::setChannelLimit(int limit)
{
    this->_limit = limit;
}

void Channel::setBanMask(std::string banmask)
{
    this->_banmask = banmask;
}

void Channel::setExceptionMask(std::string exceptionmask)
{
    this->_exceptionmask = exceptionmask;
}

void Channel::setInviteMask(std::string invitemask)
{
    this->_invitemask = invitemask;
}

void Channel::setUserLimit(int userlimit)
{
    this->_userlimit = userlimit;
}

void Channel::setChannelOperator(std::string oper)
{
    this->_operator = oper;
}

std::string Channel::getMode() const
{
    return (this->_mode);
}

int Channel::getTopic() const
{
    return (this->_topic);
}

std::string Channel::getChannelKey() const
{
    return (this->_key);
}

std::string Channel::getBanMask() const
{
    return (this->_banmask);
}

std::string Channel::getExceptionMask() const
{
    return (this->_exceptionmask);
}

std::string Channel::getInviteMask() const
{
    return (this->_invitemask);
}

int Channel::getUserLimit() const
{
    return (this->_userlimit);
}

std::string Channel::getChannelOperator() const
{
    return (this->_operator);
}

int Channel::getChannelLimit() const
{
    return (this->_limit);
}

