/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:39:28 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/08 11:51:18 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Allocates memory for a new token node, duplicates the word,
 * and initializes it.
 * Returns a pointer to the new node, or NULL on failure.
 */
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