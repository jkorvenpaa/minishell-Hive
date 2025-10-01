/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:34:40 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/01 15:06:16 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Converts the last command's exit status to a string.
 * Returns the exit status as a newly allocated string, NULL on failure.
 */
static char	*handle_exit_status(t_expansion *data, int *i)
{
	char	*result;

	result = arena_itoa(data->env_arena, data->exit_status);
	if (result)
		(*i)++;
	return (result);
}

/**
 * Handles '$?' (exit status), normal variables like '$USER',
 * undefined variables (expanded to an empty string), and invalid
 * variable names (kept literally).
 */
static char	*resolve_variable(char *token_value, int *i, t_expansion *data)
{
	char	*result;
	int		len;
	char	*var_name;
	t_env	*node;

	result = NULL;
	(*i)++;
	if (token_value[*i] == '?')
		result = handle_exit_status(data, i);
	else
	{
		var_name = get_variable_name(data->env_arena, &token_value[*i], &len);
		if (var_name)
		{
			node = get_env_node(data->env, var_name);
			if (node && node->value)
				result = node->value;
			else
				result = "";
			(*i) += len;
		}
		else
			result = "$";
	}
	return (result);
}

/**
 * Expands a variable and appends its value to the current result.
 * Returns the updated result string with expansion applied, NULL
 * on failure.
 */
char	*handle_exp_char(t_expansion *data, char *res, char *token_val, int *i)
{
	char	*new_result;
	char	*exp_value;

	exp_value = resolve_variable(token_val, i, data);
	if (!exp_value)
		return (NULL);
	new_result = ar_strjoin(data->env_arena, res, exp_value);
	if (!new_result)
		return (NULL);
	return (new_result);
}

/**
 * Main function for the loop that handles the expansion
 * of all the variables iterating through the token string.
 * Returns the expanded string or NULL on failure.
 */
static char	*expansion_loop(char *token_value, t_expansion *data)
{
	int		i;
	int		single_quotes;
	int		double_quotes;
	char	*res;

	i = 0;
	single_quotes = 0;
	double_quotes = 0;
	res = NULL;
	while (token_value[i])
	{
		handle_quote_flags(token_value[i], &single_quotes, &double_quotes);
		if (token_value[i] == '$' && !single_quotes)
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
 * Entry point for expansion: checks inputs and then processes
 * the string with expansion_loop().
 * Returns the expanded string, NULL on failure.
 */
char	*expand_value(char *token_val, t_expansion *data)
{
	char	*result;

	if (!data || !data->env_arena || !token_val)
		return (NULL);
	result = expansion_loop(token_val, data);
	return (result);
}
