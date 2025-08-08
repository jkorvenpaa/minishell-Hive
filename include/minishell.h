/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 09:25:20 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/08 14:57:46 by nmascaro         ###   ########.fr       */
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

// Tokenization functions
int	is_token_boundary(char c, int single_quote, int double_quote);
void	update_quote_flags(char c, int *single_quote, int *double_quote);
int	is_separator_symbol(char c);
t_token	*tokenize_input(char *input);


#endif
