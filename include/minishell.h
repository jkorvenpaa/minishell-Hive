/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 09:25:20 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/11 12:26:51 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>

typedef struct s_token
{
	char	*value;
	struct s_token	*next;
}	t_token;

// Linked list functions
t_token	*create_token_node(char *word);
void	append_token_to_list(t_token **head, t_token *new_node);
void	free_token_list(t_token *head);
int	add_operator_token_to_list(t_token **list, char *input, int i, int len);
int	save_token_to_list(t_token **list, char **token);

// Tokenization functions
int	is_token_boundary(char c, int single_quote, int double_quote);
int	is_separator_symbol(char c);
void	get_current_quote_state(char *input, int position, int *single_quotes, int *double_quotes);
int	get_operator_len(char *input, int i);
t_token	*tokenize_input(char *input);

#endif
