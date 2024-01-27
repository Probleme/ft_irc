/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 16:27:47 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/27 18:09:34 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <poll.h>
# include <fcntl.h>
# include <unistd.h>
# include <sstream>
# include <sys/types.h>

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# include "Numerics.hpp"

class Server;
class Channel;

class Client{
    private:
        std::string _servername;
        std::string _hostname;
        std::string _realname;
        std::string _username;
        std::string _nickname;
        std::string _message;
        bool _password;
        int _port;
        int _fd;
        std::string _command;
        time_t _time;
        bool _isRegister;
        bool _isInvited;
    public:
        Client(std::string& host, int port, int fd);
        ~Client();
        void setMessage(std::string message);
        void setNickname(std::string nickname);
        void setUsername(std::string username);
        void setRealname(std::string realname);
        void setServername(std::string servername);
        void setHostname(std::string hostname);
        void setPassword(bool password);
        void setPort(int port);
        void setFd(int fd);
        void setCommand(std::string command);
        std::string const& getMessage() const;
        std::string const& getNickname() const;
        std::string const& getUsername() const;
        std::string const& getRealname() const;
        std::string const& getServername() const;
        std::string const& getHostname() const;
        std::string const& getCommand() const;
        bool getPassword() const;
        int getPort() const;
        int getFd() const;
        
        bool isRegistered();
        void welcome();
        void reply(std::string message);
        void reply(std::string message, std::string command);
        void sendMessage();
        
        std::vector<std::string> split(std::string str, char c);
        time_t getTime() const;

        void setIsRegister(bool isRegister);
        bool getIsRegister() const;

        void setIsInvited(bool isInvited);
        bool getIsInvited() const;
};

#endif