/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 16:26:16 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/16 11:33:44 by ataouaf          ###   ########.fr       */
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
    /*
        *
    */
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
        *SO_REUSEADDR : These are the actual options being set.
            SO_REUSEADDR: Allows the socket to bind to an address that is already in use.
        *&opt: This is a pointer to the variable opt, which holds the value for the option being set. In this case, opt is set to 1, which enables the SO_REUSEADDR and SO_REUSEPORT options.
        *sizeof(opt): This specifies the size of the opt variable. It is necessary because setsockopt() needs to know the size of the buffer pointed to by &opt.
    */
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
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
        *_socket: This is the file descriptor of the server socket, obtained from the earlier socket() call.
        *(struct sockaddr *)&_client_address: This argument represents the address to which the socket is bound. The struct sockaddr is a generic socket address structure, and _client_address is likely an instance of this structure specific to the IPv4 address family (AF_INET). The (struct sockaddr *) is a typecast to the generic socket address structure pointer required by the bind() function.
        *sizeof(_client_address): This argument specifies the size of the address structure passed to bind(). It's necessary for the function to know how much memory to read when interpreting the address structure.
        
    */
    if (bind(_socket, (struct sockaddr *)&_client_address, sizeof(_client_address)) < 0)
    {
        std::cerr << "bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    /*
        *The purpose of this line is to set the server socket to listen for incoming connections.
         The listen() function tells the operating system to allow incoming connections on the socket.
        *_socket: This is the file descriptor of the server socket, obtained from the earlier socket() call.
        *10: This is the maximum number of pending connections that can be queued up at any one time. This is not the maximum number of connections that can be accepted. The maximum number of connections that can be accepted is limited by the operating system.
    */
    if (listen(_socket, 10) < 0)
    {
        std::cerr << "listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Server is listening on port " << _port << " and waiting for connections..." << std::endl;
    setDescriptors();
    while (1)
    {
        /*
            *poll is used to wait for events on the file descriptors stored in the _fds array.
             The _fds array contains the file descriptors for the server socket and all connected client sockets.
            *The second argument is the number of file descriptors to monitor. It includes the server socket and all connected client sockets.
            *The third argument is the timeout value. A value of 0 indicates that poll() should return immediately. A value of -1 indicates that poll() should block indefinitely until an event occurs.
            *When poll is called, it will block the execution of the program until one of the monitored file descriptors becomes ready.
             Once an event occurs, poll will return the number of file descriptors that have events, or it will return a negative value if an error occurs.
        */
        int poll_count = poll(_fds, _users.size() + 1, 0);
        if (poll_count < 0)
        {
            std::cerr << "poll failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < _users.size() + 1; i++)
        {
            if (_fds[i].revents == 0) // if the revents field is 0, then no events occurred on this file descriptor
                continue;
            if (_fds[i].fd == _socket) // if the server socket is ready, then a new connection is available to be accepted
                this->acceptNewConnection();
            else if (i > 0) // if a client socket is ready, then data is available to be read
                this->readFromClient(i);
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
    int flags = fcntl(fd, F_GETFL, 0); // get the file status flags 
    if (flags == -1)
    {
        std::cerr << "fcntl failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    flags |= O_NONBLOCK; // set the O_NONBLOCK flag that makes the socket non-blocking
    int s = fcntl(fd, F_SETFL, flags); // set the file status flags
    if (s == -1)
    {
        std::cerr << "fcntl failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::setDescriptors()
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
    this->_fds = new struct pollfd[this->_users.size() + 1]; // allocate memory for the pollfd array (users.size() for the clients + 1 for the server socket)
    this->_fds[0].fd = this->_socket; // add the server socket to the array
    this->_fds[0].events = POLLIN; // POLLIN is used to monitor the server socket for incoming data to read
    for (size_t i = 0; i < this->_users.size(); i++)
    {
        this->_fds[i + 1].fd = this->_users[i]->getFd(); // add the client socket to the array
        this->_fds[i + 1].events = POLLIN; // POLLIN is used to monitor the client socket for incoming data to read
    }
}

void Server::acceptNewConnection()
{
    socklen_t address_len = sizeof(_client_address);
    /*
        *accept() is used to accept a connection request from a client.
         It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket.
    */
    int socket = accept(_socket, (struct sockaddr *)&_client_address, &address_len);
    if (socket < 0)
    {
        if (errno != EWOULDBLOCK)
            std::cerr << "Error: Failed to accept connection." << std::endl;
        return;
    }
    /*
        *inet_ntoa() converts the Internet host address in, given in network byte order, to a string in IPv4 dotted-decimal notation.
        *ntohs() converts the unsigned short integer netshort from network byte order to host byte order.
    */
    std::string ip = inet_ntoa(_client_address.sin_addr); // get the IP address of the client
    int port = ntohs(_client_address.sin_port); // get the port number of the client
    if (ip.empty())
        ip = "127.0.0.1";
    this->_users.push_back(new Client(ip, this, port, socket)); // add new client to the list
    this->setNonBlocking(socket); // set the socket to non-blocking
    this->setDescriptors(); // rebuild the pollfd array
    std::cout << "New connection from " << inet_ntoa(_client_address.sin_addr) << ":" << ntohs(_client_address.sin_port) << std::endl;
}

void Server::readFromClient(int i)
{
    Client *client = _users[i - 1]; // if a client socket is ready, then data is available to be read
    char buffer[BUFFER_SIZE + 1];
    /*
        *The recv() function is used to read data from the client socket. It returns the number of bytes read, or -1 if an error occurs.
        *The purpose of this function is to read data from the client socket and handle any errors that occur.
        *argyment 1: client->getFd() is the file descriptor of the client socket.
        *argyment 2: buffer is a pointer to the buffer where the data will be stored.
        *argyment 3: sizeof(buffer) is the maximum number of bytes to read.
        *argyment 4: 0 is the type of message reception. It is set to 0 for normal behavior.
    */
    int ret = recv(client->getFd(), buffer, sizeof(buffer), 0);
    if (ret < 0)
    {
        /*
            *The purpose of this if statement is to handle any errors that occur when reading data from the client socket.
            *If the error is not EWOULDBLOCK, then the client socket has been closed or an error has occurred.
            *In either case, the client socket is removed from the list of connected clients and the pollfd array is rebuilt.
        */
        if (errno != EWOULDBLOCK) // if the errno is EWOULDBLOCK, then there is no data available to read , EWOLDBLOCK is a non-fatal error that occurs when there is no data available to read
        {
            std::cerr << "Error: recv() failed for fd " << client->getFd();
            this->removeClient(client->getFd()); // remove the client from the list
            this->setDescriptors(); // rebuild the pollfd array
        }
        return;
    }
    else if (!ret) // if ret is 0, then the connection has been closed by the client
    {
        std::cout << "Client " << client->getNickname() << " disconnected." << std::endl;
        this->removeClient(client->getFd());
        this->setDescriptors();
        return;
    }
    else
    {
        buffer[ret] = '\0'; // add the null terminator to the end of the buffer
        std::string buff = buffer;
        if (buff.at(buff.size() - 1) == '\n') // if the last character is a newline, then the client has sent a complete message
        {
            std::vector<std::string> msg = client->split(buff, '\n'); // split the buffer into separate messages by splitting on the newline character
            client->setMessage(""); // clear the client's buffer
            for (std::vector<std::string>::iterator it = msg.begin(); it != msg.end(); it++) // loop through each message
                this->handleCommands(client, *it);
        }
        else
            client->setMessage(client->getMessage() + buff); // append the message to the client's buffer until a newline is received
    }
    
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

void Server::handleCommands(Client *client, std::string &msg)
{
    std::string cmd;
    std::istringstream message(msg);
    Command command = Command();

    while (std::getline(message, cmd))
    {
        std::istringstream cmd_name(cmd); // get the command name
        std::string name;
        cmd_name >> name; // get the command name by extracting the first word from the message
        client->setCommand(name);
        /*
            The \r character, known as the carriage return, is used in programming to move the cursor back to the beginning of the line. It does not directly correspond to the action of pressing and releasing the Enter key. However, the behavior of the Enter key can vary depending on the operating system:
            In Windows, pressing the Enter key typically generates two characters: a carriage return (\r) followed by a line feed (\n). This combination is used to start a new line.
            In macOS and Linux, pressing the Enter key usually generates just a line feed (\n), which is sufficient to start a new line.
            The carriage return character can be used in various programming scenarios, such as in C where \r is used to move the cursor to the beginning of the line in console output
        */
        cmd = cmd.substr(0, cmd[cmd.length() - 1] == '\r' ? cmd.length() - 1 : cmd.length()); // remove the carriage return character because it is not part of the command
        if (command._commands.find(name) == command._commands.end()) { // if the command does not exist
            // client->reply("Invalid command");
            continue;
        }
        std::vector<std::string> args;
        std::string buf;
        std::istringstream ss(cmd.substr(name.length(), cmd.length())); // get the arguments
        while (ss >> buf)
            args.push_back(buf);
        // if (client->isRegistered())
        //     return;
        command.execute(client, args, name, this);
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

void Server::removeChannel(Channel *channel, Client *client)
{
    (void)client;
    for (size_t i = 0; i < this->_channels.size(); i++)
    {
        if (this->_channels[i] == channel)
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

