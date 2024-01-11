
#include "../inc/Channel.hpp"

Channel::Channel(std::string name, Client *client)
{
    this->_name = name;
    this->_clients.push_back(client);
    this->_topic = "";
    this->_mode = "";
    this->_key = "";
    this->_limit = 0;
    this->_banMask = "";
    this->_inviteMask = "";
    this->_exceptionMask = "";
    this->_creationTime = time(NULL);
}

Channel::~Channel()
{
}

std::string Channel::getName()
{
    return (this->_name);
}

std::string Channel::getTopic()
{
    return (this->_topic);
}

std::string Channel::getMode()
{
    return (this->_mode);
}

std::string Channel::getKey()
{
    return (this->_key);
}

int Channel::getLimit()
{
    return (this->_limit);
}

std::string Channel::getBanMask()
{
    return (this->_banMask);
}

std::string Channel::getInviteMask()
{
    return (this->_inviteMask);
}
