/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:51:27 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/08 14:58:02 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Checks if a character is a separator symbol.
 * Returns 1 if it is, 0 otherwise.
 */
int	is_separator_symbol(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

/**
 * Checks if a character marks a token boundary (a space or 
 * a separator symbol, and not inside single or double quotes)'
 * Returns 1 if it is, 0 otherwise.
 */
int	is_token_boundary(char c, int single_quote, int double_quote)
{
	if (single_quote == 0 && double_quote == 0 && (c == ' ' || is_separator_symbol(c)))
		return (1);
	return (0);
}

/**
 * Toggles single or double quote flags if the character is a quote and 
 * the opposite quote type is not active.
 */
void	update_quote_flags(char c, int *single_quote, int *double_quote)
{
	if (c == '\'' && *double_quote == 0)
		*single_quote = !(*single_quote);
	else if (c == '"' && *single_quote == 0)
		*double_quote = !(*double_quote);
}
