/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aer-raou <aer-raou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 16:26:16 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/24 15:46:54 by aer-raou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Command.hpp"

#include <cstdlib>
#include <cerrno>

class Client;
class Command;

Server::Server()
{
}

Server::Server(std::string password ,int port) : _server_name("Problem_irc"),_password(password), _start_time(dateString()), _port(port),  _socket(0), _fds()
{
}
Server::~Server() {
    for (size_t i = 0; i < this->_users.size(); i++)
    {
        if (this->_fds)
        {
            close(this->_fds[i].fd);
            delete[] this->_fds;
            this->_fds = NULL;
        }
        delete this->_users.at(i);
    }
    this->_users.clear();
    for (size_t i = 0; i < this->_channels.size(); i++)
        delete this->_channels.at(i);
    this->_channels.clear();
    delete this;
}

std::string Server::dateString()
{
    time_t now = time(0);
    std::string dt = ctime(&now);
    return (dt);
}

void Server::run()
{
    int opt = 1;
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "socket failed" << std::endl;
        return;
    }
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "setsockopt failed" << std::endl;
        close(_socket);
        return;
    }
    if (fcntl(_socket, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "fcntl failed" << std::endl;
        close(_socket);
        return;
    }
    _client_address.sin_family = AF_INET;
    _client_address.sin_port = htons(_port);
    _client_address.sin_addr.s_addr = INADDR_ANY;
    if (bind(_socket, (struct sockaddr *)&_client_address, sizeof(_client_address)) < 0)
    {
        std::cerr << "bind failed" << std::endl;
        close(_socket);
        return;
    }
    if (listen(_socket, 500) < 0)
    {
        std::cerr << "listen failed" << std::endl;
        close(_socket);
        return;
    }
    std::cout << "Server is listening on port " << _port << " and waiting for connections..." << std::endl;
    setDescriptors();
    while (1)
    {
        int poll_count = poll(_fds, _users.size() + 1, 0);
        if (poll_count < 0)
        {
            std::cerr << "poll failed" << std::endl;
            return;
        }
        for (size_t i = 0; i < _users.size() + 1; i++)
        {
            if (_fds[i].revents == 0)
                continue;
            if (_fds[i].fd == _socket)
                this->acceptNewConnection();
            else if (i > 0)
                this->readFromClient(i);
        }
    }
    close(_socket);
}

void Server::setDescriptors()
{
    if (this->_fds)
        delete[] this->_fds;
    this->_fds = new struct pollfd[this->_users.size() + 1];
    this->_fds[0].fd = this->_socket;
    this->_fds[0].events = POLLIN;
    for (size_t i = 0; i < this->_users.size(); i++)
    {
        this->_fds[i + 1].fd = this->_users.at(i)->getFd();
        this->_fds[i + 1].events = POLLIN;
    }
}

void Server::acceptNewConnection()
{
    socklen_t address_len = sizeof(_client_address);
    int socket = accept(_socket, (struct sockaddr *)&_client_address, &address_len);
    if (socket < 0)
    {
        if (errno != EWOULDBLOCK)
            std::cerr << "Error: Failed to accept connection." << std::endl;
        close(socket);
        return;
    }
    std::string ip = inet_ntoa(_client_address.sin_addr);
    int port = ntohs(_client_address.sin_port);
    this->_users.push_back(new Client(ip, port, socket));
    if (fcntl(_socket, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "fcntl failed" << std::endl;
        close(_socket);
        return;
    }
    this->setDescriptors();
    std::cout << "New connection from " << ip << ":" << port << std::endl;
}

void Server::readFromClient(int i)
{
    Client *client = _users[i - 1];
    char buffer[BUFFER_SIZE + 1];
    int ret = recv(client->getFd(), buffer, sizeof(buffer), 0);
    if (ret < 0)
    {
        if (errno != EWOULDBLOCK)
        {
            std::cerr << "Error: recv() failed for fd " << client->getFd() << std::endl;
            this->removeClient(client->getFd()); // remove the client from the list
            this->setDescriptors(); // rebuild the pollfd array
        }
        return;
    }
    else if (!ret)
    {
        std::cout << "Client " << client->getNickname() << " disconnected." << std::endl;
        this->removeClient(client->getFd());
        this->setDescriptors();
        return;
    }
    else
    {
        buffer[ret] = '\0';
        std::string buff = buffer;
        if (buff.at(buff.size() - 1) == '\n')
        {
            if (client->getMessage() != "")
            {
                buff = client->getMessage() + buff;
                client->setMessage("");
            }
            std::vector<std::string> msg = client->split(buff, '\n');
            client->setMessage("");
            for (std::vector<std::string>::iterator it = msg.begin(); it != msg.end(); it++)
                this->handleCommands(client, *it);
        }
        else
            client->setMessage(client->getMessage() + buff);
    }
}

void Server::removeClient(int fd)
{
    for (size_t i = 0; i < this->_channels.size(); i++)
    {
        for (size_t j = 0; j < this->_channels.at(i)->getClients().size(); j++)
        {
            if (this->_channels.at(i)->getClients().at(j)->getFd() == fd)
            {
                this->_channels.at(i)->removeClient(this->_channels.at(i)->getClients().at(j));
                break;
            }
        }
    }
    for (size_t i = 0; i < this->_users.size(); i++)
    {
        if (this->_users.at(i)->getFd() == fd)
        {
            close(fd);
            delete this->_users.at(i);
            this->_users.erase(this->_users.begin() + i);
            break;
        }
    }

}

bool chekIfCommandValide(std::string command)
{
    std::string command_list[] = {"NICK", "USER", "KICK", "INVITE", "TOPIC", "MODE", "JOIN", "PART", "PRIVMSG", "QUIT", "LIST", "WHO", "PING", "PONG", "NOTICE", "NAMES", "PASS"};
    for (size_t i = 0; i < 17; i++)
    {
        if (command == command_list[i])
            return (true);
    }
    return (false);
}

void Server::handleCommands(Client *client, std::string &msg)
{
    std::string cmd;
    std::istringstream message(msg);
    Command command = Command();

    while (std::getline(message, cmd))
    {
        std::istringstream cmd_name(cmd);
        std::string name;
        cmd_name >> name;
        client->setCommand(name);
        cmd = cmd.substr(0, cmd[cmd.length() - 1] == '\r' ? cmd.length() - 1 : cmd.length()); // remove the carriage return character because it is not part of the command
        if (!chekIfCommandValide(name)) {
            client->reply(ERR_UNKNOWNCOMMAND(client->getNickname(), client->getCommand()));
            return;
        }
        std::vector<std::string> args;
        std::string buf;
        std::istringstream ss(cmd.substr(name.length(), cmd.length())); // get the arguments
        while (ss >> buf)
            args.push_back(buf);
        if (!client->isRegistered() && name != "PASS")
        {
            client->reply(ERR_NOTREGISTERED(client->getNickname()));
            return;
        }
        if (client->isRegistered() && client->getUsername() == "" && client->getRealname() == "" && client->getNickname() == "*" && name != "USER" && name != "NICK" && name != "PASS")
        {
            client->reply(ERR_NOTREGISTERED(client->getNickname()));
            return;
        }
        command.execute(client, args, name, this);
        if (name == "QUIT")
            return;
        if (!client->getIsRegister() && client->getNickname() != "*" && client->getUsername() != "" && client->getRealname() != "" && (name == "USER" || name == "NICK"))
        {
            client->setIsRegister(true);
            client->reply(RPL_WELCOME(client->getNickname()));
            client->reply(RPL_YOURHOST(client->getNickname(), this->getServerName(), "1.0.0"));
            client->reply(RPL_CREATED(client->getNickname(), this->getStartTime()));
            client->reply(RPL_MYINFO(client->getNickname(), this->getServerName(), "1.0.0"));
            client->reply(RPL_ISUPPORT(client->getNickname()));
            return;
        }
    }
}

std::vector<Client *> Server::getUsers() const
{
    return this->_users;
}

std::vector<Channel *> Server::getChannels() const
{
    return this->_channels;
}

void Server::addChannel(Channel *channel, Client *client)
{
    Channel *new_channel = new Channel(channel->getName(), client);
    this->_channels.push_back(new_channel);
}

void Server:: addChannel2(Channel *channel)
{
    this->_channels.push_back(channel);
}

void Server::removeChannel(Channel *channel, Client *client)
{
    (void)client;
    for (size_t i = 0; i < this->_channels.size(); i++)
    {
        if (this->_channels.at(i) == channel)
        {
            this->_channels.erase(this->_channels.begin() + i);
            return;
        }
    }
}

void Server::addClientToChannel(Client *client, Channel *channel)
{
    channel->addClient(client);
}

void Server::removeClientFromChannel(Client *client, Channel *channel)
{
    channel->removeClient(client);
}

// void Server::sendMessageToChannel(Client *client, Channel *channel, std::string message)
// {
//     std::vector<Client *> clients = channel->getClients();
//     for (size_t i = 0; i < clients.size(); i++)
//     {
//         if (clients[i] != client)
//             clients[i]->reply(message);
//     }
// }


std::string Server::getServerName() const
{
    return this->_server_name;
}

std::string Server::getPassword()
{
    return this->_password;
}

std::string Server::getStartTime() const
{
    // the getStartTime() function returns the current time with newline character at the end of the string so we need to remove it
    std::string time = this->_start_time;
    time = time.substr(0, time.length() - 1);
    return time;
}

int Server::getPort() const
{
    return this->_port;
}

int Server::getSocket() const
{
    return this->_socket;
}

void Server::setServerName(std::string server_name)
{
    this->_server_name = server_name;
}
void Server::setPassword(std::string password)
{
    this->_password = password;
}

void Server::setStartTime(std::string start_time)
{
    this->_start_time = start_time;
}

void Server::setPort(int port)
{
    this->_port = port;
}

void Server::setSocket(int socket)
{
    this->_socket = socket;
}

void Server::setClientAddress(struct sockaddr_in client_address)
{
    this->_client_address = client_address;
}

// struct pollfd *Server::getFds() const
// {
//     return this->_fds;
// }

// void Server::setFds(struct pollfd *fds)
// {
//     this->_fds = fds;
// }

void Server::sendToAllClientsInChannel(std::string message, Channel *channel, Client *client)
{
    std::vector<Client *> clients = channel->getClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients.at(i) != client)
            clients.at(i)->reply(message);
    }
}

bool Server::checkClientPrivilege(Client *client, Channel *channel)
{
    std::vector<Client *> clients = channel->getClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients.at(i)->getNickname() == client->getNickname())
        {
            std::vector<Client *> channel_operators = channel->getChannelOperators();
            if (channel_operators.size() > 0)
            {
                for (size_t j = 0; j < channel_operators.size(); j++)
                {
                    if (channel_operators[j] == client)
                        return (true);
                }
                return (false);
            }
            else
            {
                client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()));
                return (false);
            }
        }
    }
    return (false);
}


// void Server::SendToAllClients(std::string message, Client *client)
// {
//     for (size_t i = 0; i < this->_users.size(); i++)
//     {
//         if (this->_users[i] != client)
//             this->_users[i]->reply(client->getNickname() + " " + message);
//     }
// }

void Server::sendReplyToClient(Client *client, std::string message)
{
    client->reply(message);
}

Client *Server::getClientByNickname(std::string nickname)
{
    std::vector<Client *> clients = this->getUsers();
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients.at(i)->getNickname() == nickname)
            return (clients.at(i));
    }
    return (NULL);
}


