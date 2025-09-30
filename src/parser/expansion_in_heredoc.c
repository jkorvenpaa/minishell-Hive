/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_in_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:27:19 by nmascaro          #+#    #+#             */
/*   Updated: 2025/09/30 11:37:08 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Essentially does the same as expand_value() but ignores the quotes
 * for expansions, so it also expands even if in single quotes.
 */
static char	*expand_in_heredoc(char *token_value, t_expansion *data)
{
	int	i;
	char	*result;

	if (!token_value || !data)
		return (NULL);
	i = 0;
	result = NULL;
	while (token_value[i])
	{
		if (token_value[i] == '$')
			result = handle_expansion_char(data, result, token_value, &i);
		else
		{
			result = ar_add_char_to_str(data->env_arena, result, token_value[i]);
			if (!result)
				return (NULL);
			i++;
		}
	}
	return (result);
}

/**
 * Deals with expansions inside the heredoc lines.
 * If the delimiter is quoted, returns a copy of the line as it is.
 * If it's unquoted, performs expansion (but doesn't remove quotes).
 * Returns the result.
 */
char	*hdoc_line_exp(mem_arena *ar, char *line, t_expansion *data, int hdoc_quoted)
{
	char	*result;

	result = NULL;
	if (hdoc_quoted)
		result = arena_strdup(ar, line); //no expansion, line as it is 
	else
		result = expand_in_heredoc(line, data);
	return (result);
}

