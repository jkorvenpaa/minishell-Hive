/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_logic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:51:27 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/15 15:15:00 by nmascaro         ###   ########.fr       */
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
 * a separator symbol, and not inside single or double quotes).
 * Returns 1 if it is, 0 otherwise.
 */
int	is_token_boundary(char c, int single_quote, int double_quote)
{
	if (single_quote == 0 && double_quote == 0 && (c == ' ' || is_separator_symbol(c)))
		return (1);
	return (0);
}

/**
 * Determines if the position in input is inside single or double quotes.
 * Scans from the start up to and including 'position', toggling quote states:
 * - *single_quotes = 1 if inside single quotes, else 0
 * - *double_quotes = 1 if inside double quotes, else 0
 * Quotes toggle only if not inside the opposite quote type.
 */
void	get_current_quote_state(char *input, int position, int *single_quotes, int *double_quotes)
{
	int	i;

	*single_quotes = 0;
	*double_quotes = 0;
	i = 0;
	while (i <= position)
	{
		if (input[i] == '\'' && *double_quotes == 0)
			*single_quotes = !(*single_quotes);
		else if (input[i] == '"' && *single_quotes == 0)
			*double_quotes = !(*double_quotes);
		i++;
	}
}

/**
 * Returns the length of an operator token at input.
 * Recognizes two-character operators and single-character separator symbols.
 * Returns 2, 1 or 0 if no operator found.
 */
int	get_operator_len(char *input, int i)
{
	if (input[i] == '<' && input[i + 1] && input[i + 1] == '<')
		return (2);
	if (input[i] == '>' && input[i + 1] && input[i + 1] == '>')
		return (2);
	if (is_separator_symbol(input[i]))
		return (1);
	return (0);	
}
/**
 * Determines the type of token based on the input string
 * and returns the corresponding enum value.
 */
t_token_type	identify_token(char *value)
{
	if (ft_strncmp(value, "|", 2)  == 0)
		return (PIPE);
	if (ft_strncmp(value, "<", 2)  == 0)
		return (REDIRECT_IN);
	if (ft_strncmp(value, ">", 2)  == 0)
		return (REDIRECT_OUT);
	if (ft_strncmp(value, ">>", 2)  == 0)
		return (APPEND);
	if (ft_strncmp(value, "<<", 2)  == 0)
		return (HEREDOC);
	return (WORD);
}
