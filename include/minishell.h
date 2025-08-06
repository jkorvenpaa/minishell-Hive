/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 09:25:20 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/06 15:55:27 by nmascaro         ###   ########.fr       */
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

t_token	*new_node(char *word);
void	add_to_end(t_token **head, t_token *new_node);
int	list_size(t_token *head);
void	free_list_nodes(t_token *head);
void	free_split(char **words);
t_token	*split_commands(char *input);

#endif
