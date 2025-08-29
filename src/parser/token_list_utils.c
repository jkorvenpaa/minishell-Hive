/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:39:28 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/29 09:53:49 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Allocates memory for a new token node, duplicates the word,
 * and initializes it.
 * Returns a pointer to the new node, or NULL on failure.
 */
t_token	*create_token_node(mem_arena *arena, char *word, t_token_type type)
{
	t_token	*node;

	node = arena_alloc(arena, sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = arena_strdup(arena, word);
	if (!node->value)
		return (NULL);
	node->type = type;
	node->was_quoted = 0; //needed to decide on word splitting later after expansion (we need to know if string before expansion was quoted or not)
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
 * Creates a token from a substring of input starting at i with length len, 
 * then append it to the token list. 
 * Returns 1 on success, 0 on memory failure.
 */
int	add_operator_token_to_list(mem_arena *arena, t_token **list, char *input, int i, int len)
{
	char			*token;
	t_token			*new;
	t_token_type	type;

	token = ar_substr(arena, input, i, len);
	if (!token)
		return (0);
	type = identify_token(token);
	new = create_token_node(arena, token, type);
	if (!new)
		return (0);
	append_token_to_list(list, new);
	return (1);
}

/**
 * Creates a new token node from the current token string,
 * and appends it to the list. 
 * Returns 1 on success, 0 on memory failure.
 */
int	save_token_to_list(mem_arena *arena, t_token **list, char **token, int *was_quoted)
{
	t_token			*new;
	t_token_type	type;

	if (*token)
	{
		type = identify_token(*token);
		new = create_token_node(arena, *token, type);
		if (!new)
			return (0);
		new->was_quoted = *was_quoted;
		append_token_to_list(list, new);
		*token = NULL;
		*was_quoted = 0;
	}
	return (1);
}
