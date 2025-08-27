# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/06 09:24:38 by nmascaro          #+#    #+#              #
#    Updated: 2025/08/27 12:02:36 by nmascaro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g #REMOVE -G FLAG!!!!!!!!!!!!!
SRCS = src/main.c src/parser/token_builder.c src/parser/token_logic.c \
	src/parser/token_list_utils.c src/parser/parser.c src/parser/grouping_commands.c \
	src/parser/grouping_commands_utils.c src/parser/input_validation.c \
	src/parser/env_list_utils.c src/parser/expansion.c src/parser/expansion_utils.c \
	src/parser/arena_utils.c src/parser/arena.c src/execution/execution.c \
	src/execution/command_validation.c src/execution/built_ins.c \
	src/execution/built_ins_export_unset.c 
.SECONDARY: $(OBJS)
OBJS = $(SRCS:.c=.o)
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
INCLUDES = -I$(LIBFT_DIR) -Iinclude
READLINE = -lreadline #library where the readline() functions are

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(READLINE)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	
clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	@rm -f $(OBJS)
	
fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
