/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_builder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:16:46 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/29 13:59:35 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Adds a character to the end of the current token string.
 * If string is NULL, allocates a new one with the character.
 * Returns a new string on success, NULL if failure.
 */
char	*ar_add_char_to_str(mem_arena *arena, char *s, char c)
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
 * Adds a character to the end of the current token string,
 * calling ar_add_char_to_str().
 * Returns 1 on success, 0 on memory allocation failure. 
 */
static int	add_char_to_token(mem_arena *arena, char **token, char c)
{
	char	*new_token;

	new_token = ar_add_char_to_str(arena, *token, c);
	if (!new_token)
		return (0);
	*token = new_token;
	return (1);
}

/**
 * Saves any existing token to the list, then extracts and saves
 * an operator token from the input.
 * Returns length of the operator on success, -1 on failure.
 */
static int	handle_operator_token(mem_arena *arena, char *input, int i, t_tokenizer *data)
{
	int	operator_len;
	char	*operator_str;

	operator_len = get_operator_len(input, i);
	operator_str = extract_operator_str(arena, input, i, operator_len);
	if (!operator_str)
		return (-1);
	if (!save_token_to_list(arena, &data->token_list, &data->current_token, data->was_quoted))
		return (-1);
	if (!add_operator_token_to_list(arena, &data->token_list, operator_str))
		return (-1);
	return (operator_len);
}

/**
 * Processes a char from input at index i:
 * - If an operator is found outside quotes, saves current token
 * and adds operator, returns operator length.
 * - If a token boundary is found outside quotes, saves current
 *  token and returns 0.
 * - Otherwise, adds the char to the current token.
 * Returns:
 * - number of chars to advance (1 or 2: operator len)
 * - 0 when token is saved but no skip is needed
 * - -1 on memory allocation failure
 * The final return is a safety fallback to ensure not getting stuck
 * in infinite loops and always advance, even though in normal cases earlier 
 * conditions always return first.
 */
static int	process_character(mem_arena	*arena, char *input, int i, t_tokenizer *data)
{
	int	ret_val;
	int	single_quote;
	int double_quote;

	single_quote = 0;
	double_quote = 0;
	get_curr_quote_state(input, i, &single_quote, &double_quote);
	if (is_operator_outside_quotes(input, i))
	{
		ret_val = handle_operator_token(arena, input, i, data);
		return (ret_val);
	}
	if (is_token_boundary_outside_quotes(input, i))
	{
		if (!save_token_to_list(arena, &data->token_list, &data->current_token, data->was_quoted))
			return (-1);
		return (0);
	}
	if (!ft_isspace(input[i]) || *(data->was_quoted))
	{
		if (single_quote || double_quote)
			*(data->was_quoted) = 1;
		ret_val = add_char_to_token(arena, &data->current_token, input[i]);
		return (ret_val);
	}
	return (1);
}

/**
 * Iterates through each character of the input and tokenizes it
 * into a linked list, respecting quote rules, operators and token boundaries.
 * Returns a pointer to the head of the token list on success, NULL on failure.
 */
t_token	*tokenize_input(mem_arena *arena, char *input)
{
	int		i;
	int		skip;
	t_tokenizer	data;
	int	was_quoted;

	i = 0;
	was_quoted = 0;
	if (!input || !*input)
		return (NULL);
	data.token_list = NULL;
	data.current_token = NULL;
	data.was_quoted = &was_quoted;
	while (input[i])
	{
		skip = process_character(arena, input, i, &data);
		if (skip == -1)
			return (NULL);
		if (skip == 0)
			skip = 1;
		i += skip;
	}
	if (!save_token_to_list(arena, &data.token_list, &data.current_token, data.was_quoted))
		return (NULL);
	return (data.token_list);
}
