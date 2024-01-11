
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
        std::string _topic;
        std::string _mode;
        std::string _key;
        int _limit;
        std::string _banMask;
        std::string _inviteMask;
        std::string _exceptionMask;
        time_t _creationTime;
        bool _invitOnlyChan;
        bool _secretChan;
        bool _privateChan;
        bool _topicSet;
        bool _moderatedChan;
        bool _noExternalMsg;
        bool _inviteOnlyChan;
        bool _banMaskSet;
        bool _exceptionMaskSet;
        bool _inviteMaskSet;
        bool _limitSet;
        bool _keySet;
        bool _creationTimeSet;
        bool _modeSet;
        bool _maxUserSet;
        int _maxUsers;
        std::string _password;
    public:
        Channel(std::string name, Client *client);
        ~Channel();
        std::string getName();
        std::string getTopic();
        std::string getMode();
        std::string getKey();
        int getLimit();
        std::string getBanMask();
        std::string getInviteMask();
        std::string getExceptionMask();
        time_t getCreationTime();
};

#endif