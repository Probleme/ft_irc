/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 16:26:16 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/10 11:54:46 by ataouaf          ###   ########.fr       */
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

Server::Server(std::string password ,int port) : _server_name("Problem_irc"),_password(password), _start_time(dateString()), _port(port),  _socket(0), _fds(NULL)
{
}
Server::~Server() {}

std::string Server::dateString()
{
    time_t now = time(0);
    std::string dt = ctime(&now);
    return (dt);
}

void Server::run()
{
    int opt = 1; // setsockopt option
    
    /*
        *AF_INET: Specifies the address family for the socket. In this case, AF_INET indicates that the socket will be used for IPv4 communication.
        *SOCK_STREAM: Specifies the type of socket to be created. SOCK_STREAM indicates a stream socket, which provides a reliable, connection-oriented, and byte-stream communication. It is commonly used for TCP (Transmission Control Protocol) communication.
        *0: Specifies the protocol to be used with the socket. When set to 0, the operating system chooses the most appropriate protocol automatically. For stream sockets in the AF_INET address family (IPv4), the default protocol is TCP.    */
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    /*
        *The purpose of this line is to set certain options on the _socket to modify its behavior. Specifically, it sets the SO_REUSEADDR and SO_REUSEPORT options, which allow the server to reuse local addresses and ports when binding the socket.
         This is useful for a server that needs to restart while a previous connection is still in the TIME_WAIT state, or for allowing multiple sockets to listen on the same IP address and port combination.
        *_socket: This is the file descriptor of the socket on which the options are being set. It was obtained from the previous socket() call.
        *SOL_SOCKET: This is the level at which the option is defined. SOL_SOCKET is used to specify socket-level options, as opposed to protocol-specific options.
        *SO_REUSEADDR | SO_REUSEPORT: These are the actual options being set. The | operator is used to perform a bitwise OR, which combines the two options into a single parameter.
            SO_REUSEADDR: Allows the socket to bind to an address that is already in use.
            SO_REUSEPORT: Permits multiple sockets to bind to the same port number.
        *&opt: This is a pointer to the variable opt, which holds the value for the option being set. In this case, opt is set to 1, which enables the SO_REUSEADDR and SO_REUSEPORT options.
        *sizeof(opt): This specifies the size of the opt variable. It is necessary because setsockopt() needs to know the size of the buffer pointed to by &opt.
    */
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "setsockopt failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->setNonBlocking(_socket); // make the server socket non-blocking. This means that calls to accept will return immediately if no connections are pending, rather than waiting
    // Set up the server address structure for IPv4
    _client_address.sin_family = AF_INET; // This sets the address family of the server socket to IPv4.
    _client_address.sin_addr.s_addr = INADDR_ANY; // This sets the server to accept connections on any IP address assigned to the machine.
    _client_address.sin_port = htons(_port); // This sets the port number for the server socket. htons is used to ensure that the port number is stored in network byte order.
    /*
        *The purpose of this line is to associate a local address and port with the server socket.
         Binding is a crucial step for a server as it informs the operating system on which network interface and port the server should listen for incoming connections.
        *_socket: This is the file descriptor of the server socket, obtained from the earlier socket() call.CopyCopyCopy
        *(struct sockaddr *)&_client_address: This argument represents the address to which the socket is bound. The struct sockaddr is a generic socket address structure, and _client_address is likely an instance of this structure specific to the IPv4 address family (AF_INET). The (struct sockaddr *) is a typecast to the generic socket address structure pointer required by the bind() function.
        *sizeof(_client_address): This argument specifies the size of the address structure passed to bind(). It's necessary for the function to know how much memory to read when interpreting the address structure.
        
    */
    if (bind(_socket, (struct sockaddr *)&_client_address, sizeof(_client_address)) < 0)
    {
        std::cerr << "bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(_socket, 10) < 0)
    {
        std::cerr << "listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Server is listening on port " << _port << " and waiting for connections..." << std::endl;
    constructDescriptorSet();
    while (1)
    {
        /*
            *poll is used to wait for events on the file descriptors stored in the _fds array.
             The _fds array contains the file descriptors for the server socket and all connected client sockets.
            *The second argument is the number of file descriptors to monitor. It includes the server socket and all connected client sockets.
            *The third argument is the timeout value. A value of -1 means to wait indefinitely for an event to occur.
            *When poll is called, it will block the execution of the program until one of the monitored file descriptors becomes ready.
             Once an event occurs, poll will return the number of file descriptors that have events, or it will return a negative value if an error occurs.
        */
        int poll_count = poll(_fds, _users.size() + 1, -1);
        if (poll_count < 0)
        {
            std::cerr << "poll failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < _users.size() + 1; i++)
        {
            if (_fds[i].revents == 0)
                continue;
            if (_fds[i].fd == _socket)
                this->acceptNewConnection();
            else if (i > 0)
            {
                Client *client = _users[i - 1];
                this->readFromClient(client);
            }
        }
    }
}

void Server::setNonBlocking(int fd)
{
    /*
        *The purpose of setting a socket to non-blocking mode is to ensure that I/O operations 
        (such as read() and write()) do not block the execution of your program if there is
         no data available for reading or if the output buffer is full for writing
        *We should call setNonBlocking on the server socket to make sure that
         accept() does not block the main server loop
    */
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        std::cerr << "fcntl failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    flags |= O_NONBLOCK;
    int s = fcntl(fd, F_SETFL, flags);
    if (s == -1)
    {
        std::cerr << "fcntl failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::constructDescriptorSet()
{
    // Create a single pollfd for handling all operations
    /*
        *This function called after a new connection has been accepted and added to the _users vector, 
         or after a connection has been closed and removed from the _users vector.
        *The purpose of this function is to rebuild the array of pollfd structures that will be used 
         with the poll() system call to wait for events (such as incoming data to read) on multiple sockets at once
    */
    if (this->_fds)
        delete[] this->_fds;
    this->_fds = new struct pollfd[this->_users.size() + 1];
    this->_fds[0].fd = this->_socket;
    this->_fds[0].events = POLLIN;
    for (size_t i = 0; i < this->_users.size(); i++)
    {
        this->_fds[i + 1].fd = this->_users[i]->getFd();
        this->_fds[i + 1].events = POLLIN;
    }
}

void Server::acceptNewConnection()
{
    while (1)
    {
        socklen_t address_len = sizeof(_client_address);
        int socket = accept(_socket, (struct sockaddr *)&_client_address, &address_len);
        if (socket < 0)
        {
            if (errno != EWOULDBLOCK)
                std::cerr << "Error: Failed to accept connection." << std::endl;
            break;
        }
        std::string ip = inet_ntoa(_client_address.sin_addr);
        int port = ntohs(_client_address.sin_port);
        if (ip.empty())
            ip = "127.0.0.1";
        this->_users.push_back(new Client(ip, this, port, socket)); // add new client to the list
        this->setNonBlocking(socket); // set the socket to non-blocking
        this->constructDescriptorSet(); // update the descriptor set
        std::cout << "New connection from " << inet_ntoa(_client_address.sin_addr) << ":" << ntohs(_client_address.sin_port) << std::endl;
    }
}


void Server::readFromClient(Client *client)
{
    char buffer[BUFFER_SIZE + 1];
    do
    {
        int ret = recv(client->getFd(), buffer, sizeof(buffer), 0);
        if (ret < 0)
        {
            if (errno != EWOULDBLOCK)
            {
                std::cerr << "Error: recv() failed for fd " << client->getFd();
                this->removeClient(client->getFd());
                this->constructDescriptorSet();
            }
            break;
        }
        else if (!ret)
        {
            std::cout << "Client " << client->getNickname() << " disconnected." << std::endl;
            this->removeClient(client->getFd());
            this->constructDescriptorSet();
            break;
        }
        else
        {
            buffer[ret] = '\0';
            std::string buff = buffer;
            if (buff.at(buff.size() - 1) == '\n')
            {
                std::vector<std::string> cmds = client->split(buff, '\n');
                client->setMessage("");
                for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); it++)
                    this->handleCommands(client, *it);
            }
            else
            {
                client->setMessage(client->getMessage() + buff); // append the message to the client's buffer
            }
        }
    } while (1);
    
}

void Server::removeClient(int fd)
{
    for (size_t i = 0; i < this->_users.size(); i++)
    {
        if (this->_users[i]->getFd() == fd)
        {
            delete this->_users[i];
            this->_users.erase(this->_users.begin() + i);
            break;
        }
    }
}

void Server::handleCommands(Client *client, std::string &commands)
{
    std::string cmd;
    std::istringstream message(commands);
    Command *command = new Command();

    while (std::getline(message, cmd))
    {
        cmd = cmd.substr(0, cmd[cmd.length() - 1] == '\r' ? cmd.length() - 1 : cmd.length());
        std::string name = cmd.substr(0, cmd.find(' '));
        if (command->_commands.find(name) == command->_commands.end()) {
            std::cout << "Command not found: " << name << std::endl;
            continue;
        }
        std::vector<std::string> args;
        std::string buf;
        std::istringstream ss(cmd.substr(name.length(), cmd.length()));
        while (ss >> buf)
            args.push_back(buf);
        // if (client->isRegistered())
        //     return;
        command->execute(client, args, name);
    }
}