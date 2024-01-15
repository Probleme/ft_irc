/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 12:16:52 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/14 12:54:58 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/Channel.hpp"

Channel::Channel(std::string name, Client *client)
{
    this->_name = name;
    this->_clients.push_back(client);
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
