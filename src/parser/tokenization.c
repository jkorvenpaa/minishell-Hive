/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:16:46 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/08 14:52:55 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Adds a character to the end of the given token string. If string
 * is NULL, it initializes it with the character. Otherwise, appends 
 * the character to the exiting string.
 * Returns 1 on success, 0 on memory allocation failure. 
 */
static int	add_char_to_token(char **token, char c)
{
	char	temp[2];
	char	*joined;

	temp[0] = c;
	temp[1] = '\0';
	if (!*token)
	{
		*token = ft_strdup(temp);
		if (!*token)
			return (0);
	}
	else
	{
		joined = ft_strjoin(*token, temp);
		if (!joined)
			return (0);
		free(*token);
		*token = joined;
	}
	return (1);
}
/**
 * Creates a new token node from the current token string and
 * appends it to the list. Frees the current token afterward.
 * Returns 1 on success, 0 on memory allocation failure.
 */
static int	save_token_to_list(t_token **list, char **token)
{
	t_token	*new;

	if (*token)
	{
		new = create_token_node(*token);
		if (!new)
		{
			free_token_list(*list);
			*list = NULL;
			return (0);
		}
		append_token_to_list(list, new);
		free(*token);
		*token = NULL;
	}
	return (1);
}
/**
 * Converts the symbol to a small string, creates token node, and
 * appends it to the list.
 * Returns 1 on success, 0 on memory allocation failure.
 */
static int	add_special_token_to_list(t_token **list, char symbol)
{
	char	special[2];
	t_token	*new;

	special[0] = symbol;
	special[1] = '\0';
	new = create_token_node(special);
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
 * Determines whether a character marks a token boundary or
 * should be added to the current token, and updates the list accordingly.
 * Returns 1 on success, 0 on memory allocation failure or error.
 */
static int	handle_token_logic(char c, t_token **token_list, char **current_token, int *quotes)
{
	if (is_token_boundary(c, quotes[0], quotes[1]))
	{
		if (!save_token_to_list(token_list, current_token))
			return (0);
		if (is_separator_symbol(c))
		{
			if (!add_special_token_to_list(token_list, c))
				return (0);
		}
	}
	else if (c != ' ' || quotes[0] || quotes[1])
	{
		if (!add_char_to_token(current_token, c))
		{
			free_token_list(*token_list);
			free(*current_token);
			return (0);
		}
	}
	return (1);
}
/**
 * Iterates through each character of the input, respecting quote
 * boundaries and separators, and constructs a list of tokens.
 * Returns a pointer to the head of the token list, NULL on failure.
 */
t_token	*tokenize_input(char *input)
{
	int	i;
	char	*current_token;
	t_token	*token_list;
	int	quotes[2];

	i = 0;
	current_token = NULL;
	token_list = NULL;
	quotes[0] = 0;
	quotes[1] = 0;
	while (input[i])
	{
		update_quote_flags(input[i], &quotes[0], &quotes[1]);
		if (!handle_token_logic(input[i], &token_list, &current_token, quotes))
		{
			free_token_list(token_list);
			free(current_token);
			return (NULL);
		}
		i++;
	}
	if (!save_token_to_list(&token_list, &current_token))
	{
		free_token_list(token_list);
		return (NULL);
	}
	return (token_list);
}
