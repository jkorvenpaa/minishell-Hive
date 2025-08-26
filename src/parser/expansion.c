/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:24:32 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/26 14:02:38 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char *remove_quotes(mem_arena *env_arena, char *input)
{
	int i;
	char *result;

	if (!input || !env_arena)
		return (NULL);
	i = 0;
    result = arena_strdup(env_arena, "");
    if (!result)
        return (NULL);
    while (input[i])
    {
        if (input[i] != '\'' || input[i] != '"')
		{
        	result = ar_add_char_to_str(env_arena, result, input[i]);
        	if (!result)
            	return (NULL);
		}
		i++;
    }
    return (result);
}

t_token	*expand_tokens(mem_arena *env_arena, t_token *tokens, t_env *env, int exit_status)
{
	t_token	*current;
	t_expansion data;

	data.env_arena = env_arena;
	data.env = env;
	data.exit_status = exit_status;
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

