/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:51:27 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/07 16:50:49 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token_node(char *word)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = ft_strdup(word);
	if (!node->value)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

void	append_token_to_list(t_token **head, t_token *new_node)
{
	t_token	*temp;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
}

void	free_token_list(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head->next;
		free(head->value);
		free (head);
		head = temp;
	}
}

int	is_token_boundary(char c, int single_quote, int double_quote)
{
	if (single_quote == 0 && double_quote == 0 && (c == ' ' || is_separator_symbol(c)))
		return (1);
	return (0);
}

void	update_quote_flags(char c, int *single_quote, int *double_quote)
{
	if (c == '\'' && *double_quote == 0)
		*single_quote = !(*single_quote);
	else if (c == '"' && *single_quote == 0)
		*double_quote = !(*double_quote);
}