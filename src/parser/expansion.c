/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:24:32 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/28 14:55:39 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Extracts a valid variable name from a string.
 * A variable name has to start with a letter or underscore, 
 * and can contain alphanumeric characters or underscores.
 * The length of the name is returned via 'len'.
 * Returns the newly allocated variable name or NULL if invalid.
 */
char	*get_variable_name(mem_arena *env_arena, const char *input, int *len)
{
	int	i;

	i = 0;
	if (!input || !len)
		return (NULL);
	if (!ft_isalpha(input[i]) && input[i] != '_')
	{
		*len = 0;
		return (NULL);
	}
	i++;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	*len = i;
	return (ar_substr(env_arena, input, 0, i));
}

/**
 * Main loop for the quote removal. Traverses the input string, 
 * skipping single and double quotes but preserving all other chars.
 * Returns new string without quotes, NULL on error.
 */
static char	*stripped_string(mem_arena *env_arena, char *input)
{
	int		i;
	char	*result;
	int		single_quotes;
	int		double_quotes;

	i = 0;
	result = NULL;
	single_quotes = 0;
	double_quotes = 0;
	while (input[i])
	{
		if ((input[i] == '\'' && double_quotes == 0)
			|| (input[i] == '"' && single_quotes == 0))
			handle_quote_flags(input[i], &single_quotes, &double_quotes);
		else
		{
			result = ar_add_char_to_str(env_arena, result, input[i]);
			if (!result)
				return (NULL);
		}
		i++;
	}
	return (result);
}

/**
 * Removes quotes from a string. 
 * Entry point for the stripped_string() function.
 */
static char	*remove_quotes(mem_arena *env_arena, char *input)
{
	char	*result;

	if (!input || !env_arena)
		return (NULL);
	result = stripped_string(env_arena, input);
	if (!result)
		result = arena_strdup(env_arena, ""); // result is an empty string but still a valid token
	return (result);
}

/**
 * Main function for expansion of the tokens.
 * Iterates over all tokens, and focuses on tokens which have the type WORD
 * expanding variables and the special exit status, then removes quotes.
 * Returns updated token list, NULL on failure.
 */
t_token	*exp_toks(mem_arena *env_arena, t_token *tokens, t_env *env, int status)
{
	t_token		*current;
	t_expansion	data;

	data.env_arena = env_arena;
	data.env = env;
	data.exit_status = status;
	current = tokens;
	while (current)
	{
		if (current->type == WORD && current->value)
		{
			current->value = expand_value(current->value, &data);
			if (!current->value)
				return (NULL);
			current->value = remove_quotes(env_arena, current->value);
			if (!current->value)
				return (NULL);
		}
		current = current->next;
	}
	return (tokens);
}
