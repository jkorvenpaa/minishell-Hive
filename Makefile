# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/06 09:24:38 by nmascaro          #+#    #+#              #
#    Updated: 2025/10/16 11:21:14 by nmascaro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = src/main.c src/parser/token_builder.c src/parser/token_builder_utils.c src/parser/token_logic.c \
	src/parser/token_list_utils.c src/parser/parser.c src/parser/grouping_commands.c \
	src/parser/grouping_commands_utils.c src/parser/input_validation.c \
	src/parser/env_list_utils.c src/parser/expansion.c src/parser/expansion_utils.c \
	src/parser/expansion_in_heredoc.c \
	src/parser/splitting_on_expanded_variables.c src/parser/splitting_utils.c \
	src/parser/arena_utils.c src/parser/arena.c \
	src/execution/execution.c src/execution/execute_built_ins.c \
	src/execution/execve_utils.c src/execution/built_ins.c \
	src/execution/built_ins_exit.c src/execution/built_ins_export.c \
	src/execution/built_ins_env.c \
	src/execution/files.c src/execution/execution_utils.c \
	src/parser/arena_split.c src/execution/signals.c \
	src/execution/heredoc.c src/execution/heredoc_utils.c
.SECONDARY: $(OBJS)
OBJS = $(SRCS:.c=.o)
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
INCLUDES = -I$(LIBFT_DIR) -Iinclude
READLINE = -lreadline

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
