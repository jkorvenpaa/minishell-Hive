/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_builder_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 10:57:01 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/01 14:11:38 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Checks if an operator starts at index i and is outside quotes.
 * Returns 1 if true, 0 otherwise.
 */
int	is_unquoted_operator(char *input, int i, int single_quote, int double_quote)
{
	int	operator_len;

	operator_len = get_operator_len(input, i);
	if (operator_len > 0 && single_quote == 0 && double_quote == 0)
		return (1);
	return (0);
}

/**
 * Checks if the character at index i as a token boundary and 
 * outside quotes.
 * Returns 1 if true, 0 otherwise.
 */
int	is_unquoted_boundary(char *input, int i, int single_quote, int double_quote)
{
	if (is_token_boundary(input[i], single_quote, double_quote))
		return (1);
	return (0);
}

/**
 * Extracts an operator substring from input starting at i with length len,
 * and allocates it in the arena.
 * Returns the substring, NULL if failure.
 */
char	*extract_operator_str(t_mem_arena *arena, char *input, int i, int len)
{
	char	*token;

	token = ar_substr(arena, input, i, len);
	return (token);
}

/**
 * Toggles quote flags based on the current character,
 * ensures quotes inside quotes are ignored.
 */
void	handle_quote_flags(char c, int *single_quotes, int *double_quotes)
{
	if (c == '\'' && *double_quotes == 0)
		*single_quotes = !(*single_quotes);
	else if (c == '"' && *single_quotes == 0)
		*double_quotes = !(*double_quotes);
}
