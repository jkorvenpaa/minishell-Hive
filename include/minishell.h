/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 09:25:20 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/01 15:03:27 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "arena.h"
# include "execution.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <signal.h>
# include <sys/types.h>
# include <stdbool.h>


# ifndef GLOBAL_H
#  define GLOBAL_H

extern volatile sig_atomic_t g_sig;

# endif

# define ARG_MAX 4096


typedef	enum e_token_type
{
	PIPE,
	RED_IN,
	RED_OUT,
	APPEND,
	HEREDOC,
	WORD,
}	t_token_type;

typedef struct s_token
{
	char	*value;
	t_token_type	type;
	int	was_quoted;
	struct s_token	*next;
}	t_token;

typedef struct s_command
{
	char	**argv;
	char	*infile;
	char	**outfile_list;
	char	*outfile;
	int	append;
	char	*heredoc;
	int	heredoc_quoted;
	struct s_command	*next;
} t_command;

typedef struct s_env
{
	char	*name;
	char	*value;
	struct s_env	*next;
}	t_env;

typedef struct s_expansion
{
    t_mem_arena   *env_arena;
    t_env   *env;
    int exit_status;
}   t_expansion;

typedef struct s_tokenizer
{
	t_token	*token_list;
	char	*current_token;
	int	*was_quoted;
}	t_tokenizer;

typedef struct s_parser_context
{
	t_mem_arena	*arena;
	t_mem_arena	*env_arena;
	t_env	*env;
	int	exit_status;
}	t_parser_context;

typedef struct s_shell
{
	int			exit_status; // $? fetches this
	t_mem_arena	*arena;
	t_mem_arena	*env_arena;
	t_env		*env_list;
	t_expansion	expansion;
	int			fd_in;
}	t_shell;


void	execution(t_shell *shell, t_command	*command_list);
void	exit_shell(t_shell *shell);


// Linked list functions
t_token	*create_token_node(t_mem_arena *arena, char *word, t_token_type type);
void	append_token_to_list(t_token **head, t_token *new_node);
int	add_operator_token_to_list(t_mem_arena *arena, t_token **list, char *substr);
int	save_token(t_mem_arena *ar, t_token **list, char **token, int *was_quoted);

// Tokenization functions
int	is_token_boundary(char c, int single_quote, int double_quote);
int	is_separator_symbol(char c);
void	handle_quote_flags(char c, int *single_quotes, int *double_quotes);
void	get_curr_quote_state(char *input, int pos, int *single_q, int *double_q);
int	get_operator_len(char *input, int i);
int	is_unquoted_operator(char *input, int i, int single_quote, int double_quote);
int	is_unquoted_boundary(char *input, int i, int single_quote, int double_quote);
char	*extract_operator_str(t_mem_arena *arena, char *input, int i, int len);
t_token_type	identify_token(char *value);
t_token	*tokenize_input(t_mem_arena *arena, char *input);
char	*ar_add_char_to_str(t_mem_arena *arena, char *s, char c); 
int	validate_syntax(t_token *tokens, t_shell *shell);

//Grouping the tokens into commands
t_command	*create_command_node(t_mem_arena *arena);
void	append_command_to_list(t_command **head, t_command *new_cmd);
void	*add_argument_to_argv(t_mem_arena *arena, t_command *cmd, char *arg);
t_command	*group_commands(t_mem_arena *arena, t_token *tokens);
void	outfile_to_list(t_mem_arena *arena, t_command *cmd, const char *name);

// Word splitting functions
t_token	*split_expanded_variables(t_mem_arena *arena, t_token *tokens);
t_token	*remove_empty_tokens(t_token *token);
int	is_only_spaces(const char *str);
int	get_next_word(char *str, int *start, int *end);
int	needs_splitting(t_token *token);

// Expansion functions
t_token	*exp_toks(t_shell *shell, t_token *tokens);
char	*get_variable_name(t_mem_arena *env_arena, const char *input, int *len);
char *expand_value(char *token_val, t_expansion *data);
char	*handle_exp_char(t_expansion *data, char *res, char *token_val, int *i);
char	*hd_line_exp(t_mem_arena *ar, char *line, t_expansion *dt, int hd_quot);

// Environment list functions
t_env	*get_env_node(t_env *env_list, const char *name);
t_env	*init_env_list(t_mem_arena *env_arena, const char **envp);

//main function in parsing branch
t_command	*run_parser(char *input, t_parser_context *data, t_shell *shell);
char	*remove_quotes(t_mem_arena *env_arena, char *input);

#endif
