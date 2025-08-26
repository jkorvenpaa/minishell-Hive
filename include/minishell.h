/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 09:25:20 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/26 11:04:45 by nmascaro         ###   ########.fr       */
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
typedef struct s_expansion
{
    mem_arena   *env_arena;
    t_env   *env;
    int exit_status;
}   t_expansion;

//execution transfer
t_shell	*init_shell(mem_arena *arena, mem_arena *env_arena, char **envp);
void	execution(t_shell *shell, t_command	*command_list);

void	check_if_builtin(t_command *command, t_shell *shell);

int		echo(t_command *command);
int		cd(char *next_cmd);
int 	pwd(char *cmd);

int		export(char	*next_cmd, t_shell *shell);
t_env	*new_env(t_env *new, t_shell *shell, char *next_cmd);
t_env	*update_env(t_env *new, t_shell *shell, char *next_cmd);
int		unset(char *next_cmd, t_shell *shell);
int	env_builtin(t_shell *shell);



// Linked list functions
t_token	*create_token_node(mem_arena *arena, char *word, t_token_type type);
void	append_token_to_list(t_token **head, t_token *new_node);
int	add_operator_token_to_list(mem_arena *arena, t_token **list, char *input, int i, int len);
int	save_token_to_list(mem_arena *arena, t_token **list, char **token);

// Tokenization functions
int	is_token_boundary(char c, int single_quote, int double_quote);
int	is_separator_symbol(char c);
void	handle_quote_flags(char c, int *single_quotes, int *double_quotes);
void	get_current_quote_state(char *input, int position, int *single_quotes, int *double_quotes);
int	get_operator_len(char *input, int i);
t_token_type	identify_token(char *value);
t_token	*tokenize_input(mem_arena *arena, char *input);
char	*ar_add_char_to_str(mem_arena *arena, char *s, char c); 

//Grouping the tokens into commands
t_command	*create_command_node(mem_arena *arena);
void	append_command_to_list(t_command **head, t_command *new_cmd);
void	*add_argument_to_argv(mem_arena *arena, t_command *cmd, char *arg);
t_command	*group_commands(mem_arena *arena, t_token *tokens);

int	validate_syntax(t_token *tokens);

// Expansion functions
t_token	*expand_tokens(mem_arena *env_arena, t_token *tokens, t_env *env, int exit_status);
char	*get_variable_name(mem_arena *env_arena, const char *input, int *len);
char *expand_value(char *token_val, t_expansion *data);

// Environment list functions
t_env	*get_env_node(t_env *env_list, const char *name);
t_env	*init_env_list(mem_arena *env_arena, char **envp);

// Arena functions (maybe in arena.h???)
char	*arena_strdup(mem_arena *arena, const char *str);
char	*ar_substr(mem_arena *arena, char const *s, unsigned int st, size_t len);
char	*ar_strjoin(mem_arena *arena, char const *s1, char const *s2);
char	*arena_itoa(mem_arena *env_arena, int n);

//main function in parsing branch
t_command	*run_parser(mem_arena *arena, mem_arena *env_arena); 

#endif
