# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kmeeseek <kmeeseek@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/16 20:21:08 by kmeeseek          #+#    #+#              #
#    Updated: 2021/07/25 21:50:13 by kmeeseek         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
HEAD = *.h
INPUT = $(wildcard ./*.c)
FLAGS = -lpthread
CFLAGS	= -Wall -Wextra -Werror
RM = rm -f
OBJS = $(INPUT:.c=.o)

all: $(NAME)
$(NAME): $(OBJS) $(HEAD)
	gcc -g $(OBJS) $(CFLAGS) $(FLAGS) -o $(NAME)
%.o: %.c %.h
	gcc -g $(CFLAGS) -c $< -o $(<:.c=.o) -fsanitize=address
clean:
	@$(RM) $(OBJS)
fclean: clean
	@$(RM) $(NAME)
re: fclean all
.PHONY: all clean fclean re