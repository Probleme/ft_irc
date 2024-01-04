/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataouaf <ataouaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 14:02:06 by ataouaf           #+#    #+#             */
/*   Updated: 2024/01/03 21:14:35 by ataouaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./ft_irc [port] [password]" << std::endl;
        return (1);
    }
    for (int i = 0; argv[1][i]; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            std::cout << "Port must be a number" << std::endl;
            return (1);
        }
    }
    int port = atoi(argv[1]);
    std::string password = argv[2];
    Server server(password, port);
    server.run();
    return (0);
}