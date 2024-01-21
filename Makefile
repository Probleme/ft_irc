# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aer-raou <aer-raou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/31 16:22:29 by ataouaf           #+#    #+#              #
#    Updated: 2024/01/21 09:25:19 by aer-raou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = c++ -Wall -Wextra -Werror -g -fsanitize=address,undefined
NAME = ircserv
OBJECTS = ./obj
INCLUDES = ./inc
SRCS = $(addprefix src/, Server.cpp main.cpp Client.cpp Command.cpp Channel.cpp bot.cpp)
OBJS = $(addprefix $(OBJECTS)/, $(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJECTS)/%.o: %.cpp $(addprefix $(INCLUDES)/, Server.hpp Client.hpp Command.hpp Channel.hpp Numerics.hpp)
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJECTS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

