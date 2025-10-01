/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:39:28 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/01 14:17:35 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Allocates memory for a new token node, duplicates the word,
 * assigns its type, and initializes fields.
 * Returns a pointer to the new node, or NULL on failure.
 */
t_token	*create_token_node(t_mem_arena *arena, char *word, t_token_type type)
{
	t_token	*node;

	node = arena_alloc(arena, sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = arena_strdup(arena, word);
	if (!node->value)
		return (NULL);
	node->type = type;
	node->was_quoted = 0;
	node->next = NULL;
	return (node);
}

/**
 * Appends a token node to the end of a linked list.
 * If the list is empty, new node becomes the  head.
 * Otherwise, it's added to the end.
 */
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

/**
 * Creates a token node from an operator substring
 * and appends it to the token list. 
 * Returns 1 on success, 0 on memory failure.
 */
int	add_operator_token_to_list(t_mem_arena *arena, t_token **list, char *substr)
{
	t_token			*new;
	t_token_type	type;

	if (!substr)
		return (0);
	type = identify_token(substr);
	new = create_token_node(arena, substr, type);
	if (!new)
		return (0);
	append_token_to_list(list, new);
	return (1);
}

/**
 * Finalizes the current token string by creating a node and
 * and appending it to the list. Resets token and was_quoted flag.
 * Returns 1 on success, 0 on memory failure.
 */
int	save_token(t_mem_arena *ar, t_token **list, char **token, int *was_quoted)
{
	t_token			*new;
	t_token_type	type;

	if (*token)
	{
		type = identify_token(*token);
		new = create_token_node(ar, *token, type);
		if (!new)
			return (0);
		new->was_quoted = *was_quoted;
		append_token_to_list(list, new);
		*token = NULL;
		*was_quoted = 0;
	}
	return (1);
}
