/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitting_on_expanded_variables.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:35:29 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/01 15:48:39 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Creates a new WORD token from a substring of a given string.
 * Returns a pointer to the new token, NULL if allocation fails.
 */
static t_token	*new_word_token(t_mem_arena *arena, char *s, int start, int len)
{
	char	*word;
	t_token	*token;

	word = ar_substr(arena, s, start, len);
	if (!word)
		return (NULL);
	token = create_token_node(arena, word, WORD);
	if (!token)
		return (NULL);
	token->was_quoted = 0;
	token->next = NULL;
	return (token);
}

/**
 * Splits the value of a token into multiple tokens, one for each word.
 * Returns the head of the new list of word tokens, or NULL if allocation fails.
 */
static t_token	*split_token_words(t_mem_arena *arena, t_token *token)
{
	int		start;
	int		end;
	t_token	*head;
	t_token	*new_token;

	start = 0;
	head = NULL;
	while (get_next_word(token->value, &start, &end))
	{
		new_token = new_word_token(arena, token->value, start, end - start);
		if (!new_token)
			return (head);
		append_token_to_list(&head, new_token);
		start = end;
	}
	return (head);
}

/**
 * Replaces a single token in a linked list with a list of tokens.
 * Inserts the new list of tokens (split_tokens) in place of the original
 * current token in the linked list. It also updates the previous token,
 * and connects the end of the new list to the remainder of the original one. hol
 */
static void	new_lst(t_token **head, t_token **prv, t_token *cr, t_token *spl_tk)
{
	t_token	*tail;

	tail = spl_tk;
	while (tail->next)
		tail = tail->next;
	if (*prv)
		(*prv)->next = spl_tk;
	else
		*head = spl_tk;
	tail->next = cr->next;
	*prv = tail;
}

/**
 * Main entry point for splitting.
 * Iterates through a token list and splits all tokens that require it.
 * Ensures avoiding infinite loop by moving forward if split_token_words()
 * returns NULL (for example if it's empty token).
 * Returns the head of the updated token list.
 */
t_token	*split_expanded_variables(t_mem_arena *arena, t_token *tokens)
{
	t_token	*current;
	t_token	*prev;
	t_token	*head;
	t_token	*split_tokens;

	current = tokens;
	prev = NULL;
	head = tokens;
	while (current)
	{
		if (current->type == WORD && needs_splitting(current))
		{
			split_tokens = split_token_words(arena, current);
			if (split_tokens)
				new_lst(&head, &prev, current, split_tokens);
			else
				prev = current;
		}
		else
			prev = current;
		current = prev->next;
	}
	return (head);
}

/**
 * Removes tokens that have an empty value and were not quoted.
 * Used after variable expansion and word splitting.
 * Updates the linked list to skip removed tokens.
 */
t_token	*remove_empty_tokens(t_token *token)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*head;

	head = token;
	prev = NULL;
	curr = token;
	while (curr)
	{
		if (curr->value && is_only_spaces(curr->value) && !curr->was_quoted)
		{
			if (prev)
				prev->next = curr->next;
			else
				head = curr->next;
			curr = curr->next;
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
	return (head);
}
