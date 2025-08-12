/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:39:28 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/12 15:57:38 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Allocates memory for a new token node, duplicates the word,
 * and initializes it.
 * Returns a pointer to the new node, or NULL on failure.
 */
t_token	*create_token_node(char *word, t_token_type type)
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
	node->type = type;
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
 * Frees the entire token list and its contents.
 */
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

/**
 * Creates a token from a substring of input starting at i with length len, 
 * then append it to the token list. 
 * On memory failure, frees the entire list and sets *list to NULL.
 * Returns 1 on success, 0 on failure.
 */
int	add_operator_token_to_list(t_token **list, char *input, int i, int len)
{
	char	*token;
	t_token	*new;
	t_token_type	type;

	token = malloc(len + 1);
	if (!token)
	{
		free_token_list(*list);
		*list = NULL;
		return (0);
	}
	ft_strlcpy(token, &input[i], len + 1);
	type = identify_token(token);
	new = create_token_node(token, type);
	free(token);
	if (!new)
	{
		free_token_list(*list);
		*list = NULL;
		return (0);
	}
	append_token_to_list(list, new);
	return (1);
}

/**
 * Creates a new token node from the current token string and appends it to the list. 
 * On memory failure, frees list and token and sets them to NULL.
 * Returns 1 on success, 0 failure.
 */
int	save_token_to_list(t_token **list, char **token)
{
	t_token	*new;
	t_token_type	type;

	if (*token)
	{
		type = identify_token(*token);
		new = create_token_node(*token, type);
		if (!new)
		{
			free_token_list(*list);
			free(*token);
			*list = NULL;
			*token = NULL;
			return (0);
		}
		append_token_to_list(list, new);
		free(*token);
		*token = NULL;
	}
	return (1);
}
