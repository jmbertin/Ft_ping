# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbertin <jbertin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 17:41:24 by jbertin           #+#    #+#              #
#    Updated: 2023/11/30 22:12:00 by jbertin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

UP = "\033[A"
CLEAR = "\033[K"

NAME	= ft_ping
SRCS	= main.c ft_tools.c ft_parsing.c ft_ping.c ft_display.c

LIBFT_A = libft.a
LIBFT_DIR = Libft/
LIBFT  = $(addprefix $(LIBFT_DIR), $(LIBFT_A))

OBJS	= ${SRCS:.c=.o}
CC = gcc
FLAGS = -Wall -Wextra -Werror
INCLUDE = -lm -I includes/

all : ${NAME}

.c.o :
		@${CC} ${FLAGS} -c $< -o $@ ${INCLUDE}
		@echo $(UP)$(CLEAR)"\033[0;93mCompiling ${NAME} : $@ \033[0;39m"

${NAME}	: ${OBJS}
		@make -C $(LIBFT_DIR)
		@${CC} -o ${NAME} ${OBJS} ${INCLUDE} $(LIBFT)
		@echo $(UP)$(CLEAR)"\033[0;92m${NAME} compiled !\033[0;39m"

clean :
		@make -C $(LIBFT_DIR) clean
		@rm -f ${OBJS}
		@echo "\033[0;92m${NAME} objects files cleaned.\033[0;39m"

fclean : clean
		@make -C $(LIBFT_DIR) fclean
		@rm -f ${NAME}
		@echo "\033[0;92m${NAME} cleaned.\033[0;39m"

test :
		docker build -t ft_ping_test .
		docker rm -f ft_ping_docker || true
		docker run -it --rm --name ft_ping_docker ft_ping_test

re : fclean all
		@echo "\033[0;92m${NAME} cleaned and rebuilded.\033[0;39m"
