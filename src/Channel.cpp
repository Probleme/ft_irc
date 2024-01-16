/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:27:25 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/16 11:27:26 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/Channel.hpp"

Channel::Channel(std::string name, Client *client)
{
    this->_name = name;
    this->_clients.push_back(client);
    //get the current time and save it in the _ChannelCreatedAt
    this->_ChannelCreatedAt = time(0);
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