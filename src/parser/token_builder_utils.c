/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_builder_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 10:57:01 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/29 14:03:22 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Checks if an operator starts at index i and is outside quotes.
 * Returns 1 if true, 0 otherwise.
 */
int	is_operator_outside_quotes(char *input, int i)
{
	int	operator_len;
	int	single_quote;
	int	double_quote;

	operator_len = get_operator_len(input, i);
	get_curr_quote_state(input, i, &single_quote, &double_quote);
	if (operator_len > 0 && single_quote == 0 && double_quote == 0)
		return (1);
	return (0);
}

/**
 * Checks if the character at index i as a token boundary and 
 * outside quotes.
 * Returns 1 if true, 0 otherwise.
 */
int	is_token_boundary_outside_quotes(char *input, int i)
{
	int	single_quote;
	int	double_quote;

	get_curr_quote_state(input, i, &single_quote, &double_quote);
	if (is_token_boundary(input[i], single_quote, double_quote))
		return (1);
	return (0);
}

/**
 * Extracts an operator substring from input starting at i with length len,
 * and allocates it in the arena.
 * Returns the substring, NULL if failure.
 */
char	*extract_operator_str(mem_arena *arena, char *input, int i, int len)
{
	char	*token;

	token = ar_substr(arena, input, i, len);
	return (token);
}
