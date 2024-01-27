/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 16:26:28 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/27 18:09:58 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
# include <sys/types.h>
# include <sys/time.h>


# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
#include "Channel.hpp"
#include "Numerics.hpp"

# define BUFFER_SIZE 1024

class Client;
class Channel;

class Server
{
    private:
        std::string _server_name;
        std::string _password;
        std::string _start_time;
        int _port;
        int _socket;
        struct sockaddr_in _address;
        struct pollfd *_fds;
        std::vector<Channel *> _channels;
        std::vector<Client *> _users;
    public:
        Server();
        Server(std::string password, int port);
        ~Server();

        std::string dateString();
        void run();
        std::string getServerName() const;
        std::string getPassword() ;
        std::string getStartTime() const;
        int getPort() const;
        int getSocket() const;
        struct sockaddr_in getClientAddress() const;
        struct pollfd *getFds() const;
        void setServerName(std::string server_name);
        void setPassword(std::string password);
        void setStartTime(std::string start_time);
        void setPort(int port);
        void setSocket(int socket);
        void setClientAddress(struct sockaddr_in client_address);
        void setFds(struct pollfd *fds);

        void setNonBlocking(int fd);
        void setDescriptors();
        void acceptNewConnection();
        void addNewClient(int fd, std::string ip, int port);
        void readFromClient(int i);
        void removeClient(int fd, Server *server);
        void handleCommands(Client *client, std::string &command);
        std::vector<Client *> getUsers() const;
        std::vector<Channel *> getChannels() const;
        void addChannel(Channel *channel, Client *client);
        void removeChannel(Channel *channel, Client *client);
        void addClientToChannel(Client *client, Channel *channel);
        void removeClientFromChannel(Client *client, Channel *channel);
        void sendToAllClientsInChannel(std::string message, Channel *channel, Client *client);
        void addChannel2(Channel *channel);
        bool checkClientPrivilege(Client *client, Channel *channel);
        void sendReplyToClient(Client *client, std::string message);
        Client *getClientByNickname(std::string nickname);
        void sendReplyToClient2(Client *client, std::string message, Client *client2);


};


# endif
