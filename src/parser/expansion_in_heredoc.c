/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_in_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:27:19 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/01 15:03:33 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Essentially does the same as expand_value() but ignores the quotes
 * for expansions, so it also expands even if in single quotes.
 */
static char	*expand_in_heredoc(char *token_value, t_expansion *data)
{
	int		i;
	char	*res;

	if (!token_value || !data)
		return (NULL);
	i = 0;
	res = NULL;
	while (token_value[i])
	{
		if (token_value[i] == '$')
			res = handle_exp_char(data, res, token_value, &i);
		else
		{
			res = ar_add_char_to_str(data->env_arena, res, token_value[i]);
			if (!res)
				return (NULL);
			i++;
		}
	}
	return (res);
}

/**
 * Deals with expansions inside the heredoc lines.
 * If the delimiter is quoted, returns a copy of the line as it is.
 * If it's unquoted, performs expansion (but doesn't remove quotes).
 * Returns the result.
 */
char	*hd_line_exp(t_mem_arena *ar, char *line, t_expansion *dt, int hd_quot)
{
	char	*result;

	result = NULL;
	if (hd_quot)
		result = arena_strdup(ar, line);
	else
		result = expand_in_heredoc(line, dt);
	return (result);
}
