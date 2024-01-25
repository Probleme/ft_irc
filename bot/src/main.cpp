/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 09:41:40 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/23 11:04:23 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/randomMsg.hpp"

std::vector<std::string> split(std::string str, char c)
{
    std::vector<std::string> res;
    std::string tmp;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str.at(i) == c)
        {
            res.push_back(tmp);
            tmp.clear();
        }
        else
            tmp += str.at(i);
    }
    res.push_back(tmp);
    return (res);
}

std::string dateString()
{
    time_t now = time(0);
    std::string dt = ctime(&now);
    dt = dt.substr(0, dt.size() - 1);
    return (dt);
}

void execute(std::string message)
{
    std::vector<std::string> args = split(message, ' ');
    
    if (args.at(0) == "help")
    {
        std::cout << "Available commands for bot : " << std::endl;
        std::cout << "help : print all commands available for bot and all commands available for client" << std::endl;
        std::cout << "logtime <client> : return time since client is loged in minutes and seconds" << std::endl;
        std::cout << "random_msg <client> : send a random message to a client" << std::endl;
        std::cout << "random_msg example: " << std::endl;
        std::cout << "\tSalutations! I'm your trusty bot companion. <nickname> has a message just for you." << std::endl;
        std::cout << "\tHey! I'm the talkative bot. <nickname> has a message waiting for you.." << std::endl;
        std::cout << "\tGreetings, human! I'm here to assist. <nickname> is ready for a chat with you." << std::endl;
        std::cout << "is_logged <client> : check if a client is logged" << std::endl;
    }
    else if (args.at(0) == "logtime")
    {
        
    }
    
}

int main(int argc, char **argv)
{
    int sock;
    struct sockaddr_in address;

    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " hostname port" << std::endl;
        return (1);
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cout << "Socket error" << std::endl;
        return (1);
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[2]));
    address.sin_addr.s_addr = INADDR_ANY;
    if (connect(sock, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        std::cout << "Connect error" << std::endl;
        close(sock);
        return (1);
    }
    std::cout << "Connected to " << argv[1] << ":" << argv[2] << std::endl;
    std::string msg;
    std::cout << "Welclome to the chatbot. Type 'help' to list commands available by this chatbot"<< std::endl;
    std::string start_time = dateString();
    while (1)
    {
        std::getline(std::cin, msg);
        if (msg == "exit")
            break;
        send(sock, msg.c_str(), msg.size(), 0);
        char buffer[BUFFER_SIZE + 1];
        int n = recv(sock, buffer , sizeof(buffer), 0);
        buffer[n] = '\0';
        std::cout << "Server response: " << buffer << std::endl;
        
    }
    close(sock);
    return (0);
}