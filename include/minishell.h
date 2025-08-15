/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 09:25:20 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/15 09:39:35 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>

typedef	enum e_token_type
{
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	WORD,
}	t_token_type;
typedef struct s_token
{
	char	*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_command
{
	char	**argv;
	char	*infile;
	char	*outfile;
	int	append;
	char	*heredoc;
	struct s_command	*next;
} t_command;

// Linked list functions
t_token	*create_token_node(char *word, t_token_type type);
void	append_token_to_list(t_token **head, t_token *new_node);
void	free_token_list(t_token *head);
int	add_operator_token_to_list(t_token **list, char *input, int i, int len);
int	save_token_to_list(t_token **list, char **token);

// Tokenization functions
int	is_token_boundary(char c, int single_quote, int double_quote);
int	is_separator_symbol(char c);
void	get_current_quote_state(char *input, int position, int *single_quotes, int *double_quotes);
int	get_operator_len(char *input, int i);
t_token_type	identify_token(char *value);
t_token	*tokenize_input(char *input);

//Grouping the tokens into commands
t_command	*create_command_node(void);
void	append_command_to_list(t_command **head, t_command *new_cmd);
void	*add_argument_to_argv(t_command *cmd, char *arg);
t_command	*group_commands(t_token *tokens);

void	run_parser(void); //main function in parsing branch

#endif
