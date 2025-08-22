/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_builder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:16:46 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/22 10:52:31 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ar_add_char_to_str(mem_arena *arena, char *s, char c) //also used in expansions.c (need to move it somewhere else)
{
	size_t	len;
	char	*result;

	if (!s)
		len = 0;
	else
		len = ft_strlen(s);
	result = arena_alloc(arena, len + 2);
	if (!result)
		return (NULL);
	if (s)
		ft_memcpy(result, s, len);
	result[len] = c;
	result[len + 1] = '\0';
	return (result);
}
/**
 * Adds a character to the end of the current token string. If string
 * is NULL, allocates a new string with the character. Otherwise, appends 
 * the character to the exiting string.
 * Returns 1 on success, 0 on memory allocation failure. 
 */
static int	add_char_to_token(mem_arena *arena, char **token, char c)
{
	char *new_token;

	new_token = ar_add_char_to_str(arena, *token, c);
	if (!new_token)
		return (0);
	*token = new_token;
	return (1);
}

/**
 * Saves any existing token to the list, then adds the operator token.
 * Returns the length of the operator token on success, or -1 on
 * memory allocation failure.
 */
static int	handle_operator_token(mem_arena *arena, char *input, int i, t_token **list, char **token)
{
	int	operator_len;

	operator_len = get_operator_len(input, i);
	if (!save_token_to_list(arena, list, token))
			return (-1);
	if (!add_operator_token_to_list(arena, list, input, i, operator_len))
			return (-1);
	return (operator_len);
}

/**
 * Adds current char to the token, handling memory errors by freeing 
 * the token list and token itself. Returns 1 on success, -1 on failure.
 */
/* static int	char_addition_cleanup(mem_arena *arena, char **token, char c, t_token **list) //probably dont need this function anymore!
{
	if (!add_char_to_token(arena, token, c))
	{
		free_token_list(*list);
		*list = NULL;
		free(*token);
		*token = NULL;
		return (-1);
	}
	return (1);
}
*/ 
/**
 * Processes a char from input at index i with respect to quoting and operators.
 * - If an operator is detected and not inside quotes,
 * handles it and returns operator length.
 * - If a char is a token boundary, saves current token and returns 0.
 * - Otherwise, adds the char to the current token.
 * Returns:
 * - number of chars to advance (1 or operator length)
 * - 0 if a token boundary is hit (token saved, but no skip)
 * - -1 on memory allocation failure
 * The final return is a safety fallback to ensure not getting stuck
 * in infinite loops and always advance, even though in normal cases earlier 
 * conditions always return first.
 */
static int	process_character(mem_arena	*arena, char *input, int i, t_token **token_list, char **token)
{
	int	operator_len;
	int single_quote;
	int	double_quote;
	int	ret_val;
	
	get_current_quote_state(input, i, &single_quote, &double_quote);
	operator_len = get_operator_len(input, i);
	if (operator_len > 0 && single_quote == 0 && double_quote == 0)
	{
		ret_val = handle_operator_token(arena, input, i, token_list, token);
		return (ret_val);
	}
	if (is_token_boundary(input[i], single_quote, double_quote))
	{
		if (!save_token_to_list(arena, token_list, token))
			return (-1);
		return (0);
	}
	if (input[i] != ' ' || single_quote || double_quote)
	{
		ret_val = add_char_to_token(arena, token, input[i]);
		return (ret_val);
	}
	return (1);
}
/**
 * Iterates through each character of the input, respecting quote
 * rules and token boundaries.
 * Returns a pointer to the head of the token list on success, NULL on failure.
 */
t_token	*tokenize_input(mem_arena *arena, char *input)
{
	int	i;
	int	skip;
	char	*current_token;
	t_token	*token_list;

	i = 0;
	current_token = NULL;
	token_list = NULL;
	while (input[i])
	{
		skip = process_character(arena, input, i, &token_list, &current_token);
		if (skip == -1)
		{
			//free_token_list(token_list);
			//free(current_token);
			return (NULL);
		}
		if (skip == 0)
			skip = 1;
		i += skip;
	}
	if (!save_token_to_list(arena, &token_list, &current_token))
	{
		//free_token_list(token_list);
		return (NULL);
	}
	return (token_list);
}
