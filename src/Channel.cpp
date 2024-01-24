/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aer-raou <aer-raou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:27:25 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/24 15:42:02 by aer-raou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/Channel.hpp"

Channel::Channel(std::string name, Client *client)
{
    this->_name = name;
    this->_clients.push_back(client);
    this->_operator.push_back(client);
    this->_ChannelCreatedAt = time(0);
    this->_mode = "";
    this->_topic = "";
    this->_key = "";
    this->_banmask = "";
    this->_exceptionmask = "";
    this->_invitemask = 0;
    this->_userlimit = 100;
    this->_limit = 100;
}

Channel::~Channel()
{
    // we need to delete all clients in the channel
    for (size_t i = 0; i < this->_clients.size(); i++)
        delete this->_clients.at(i);
    this->_clients.clear();
    // we need to delete all operators in the channel
    for (size_t i = 0; i < this->_operator.size(); i++)
        delete this->_operator.at(i);
    this->_operator.clear();
    delete this;
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
        if (this->_clients.at(i) == client)
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

void Channel::setTopic(std::string topic)
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

void Channel::setInviteMask(int invitemask)
{
    this->_invitemask = invitemask;
}

void Channel::setUserLimit(int userlimit)
{
    this->_userlimit = userlimit;
}

void Channel::AddChannelOperator(Client *oper)
{
    if (this->CheckClientIsOperator(oper->getNickname()))
        return;
    this->_operator.push_back(oper);
}

void Channel::RemoveChannelOperator(Client *oper)
{
    for (size_t i = 0; i <= this->_operator.size(); i++)
    {
        if (this->_operator.at(i)->getNickname() == oper->getNickname())
        {
            this->_operator.erase(this->_operator.begin() + i);
            return;
        }
    }
}

std::string Channel::getMode() const
{
    return (this->_mode);
}

std::string Channel::getTopic() const
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

int Channel::getInviteMask() const
{
    return (this->_invitemask);
}

int Channel::getUserLimit() const
{
    return (this->_userlimit);
}

std::vector<Client *> Channel::getChannelOperators() const
{
    return (this->_operator);
}

int Channel::getChannelLimit() const
{
    return (this->_limit);
}

std::string Channel::getTopicTime() const
{
    return (this->_topicTime);
}

void Channel::setTopicTime(std::string topicTime)
{
    this->_topicTime = topicTime;
}

bool Channel::CheckClientIsOperator(std::string nickname)
{
    for (size_t i = 0; i < this->_operator.size(); i++)
    {
        if (this->_operator.at(i)->getNickname() == nickname)
            return (true);
    }
    return (false);
}

Client *Channel::getClient(std::string nickname)
{
    for (size_t i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients.at(i)->getNickname() == nickname)
            return (this->_clients.at(i));
    }
    return (NULL);
}


void Channel::replyToAllUsersInChannel(std::string message, Client *client)
{
    for (size_t i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients.at(i)->getNickname() != client->getNickname())
        {
            this->_clients.at(i)->setMessage(message);
            this->_clients.at(i)->sendMessage();
        }
    }
}
