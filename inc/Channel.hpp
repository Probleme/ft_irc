
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
    public:
        Channel(std::string name, Client *client);
        ~Channel();
        std::string setName(std::string name);
        std::string const getName() const;
        std::vector<Client *> getClients();
        void addClient(Client *client);
        void removeClient(Client *client);

};

#endif