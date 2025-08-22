/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:24:32 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/22 15:53:56 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_env_to_list(t_env **head, t_env *new_node) //maybe static?
{
	t_env	*temp;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;	
}

t_env	*init_env_list(mem_arena *env_arena, char **envp)
{
	int	i;
	t_env *head;
	t_env *new_env;
	char *equal_sign;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		new_env = arena_alloc(env_arena, sizeof(t_env));
		if (!new_env)
			return (NULL);
		ft_bzero(new_env, sizeof(t_env)); //zero all fields of t_env
		equal_sign = ft_strchr(envp[i], '=');  //search for = and set pointer there
		if (equal_sign)
		{
			new_env->name = ar_substr(env_arena, envp[i], 0, equal_sign - envp[i]); //before = (p.ex: USER) copies before (but its always set to before the =))
			new_env->value = arena_strdup(env_arena, equal_sign + 1); // after = (p.ex.: guest) copies everything after equal sign
			if (!new_env->name || !new_env->value)
				return (NULL);
		}
		else //if no value is assigned, the environment variable still exists but just with empty value
		{
			new_env->name = arena_strdup(env_arena, envp[i]);
			new_env->value = arena_strdup(env_arena, "");
			if (!new_env->name || !new_env->value)
				return (NULL);
		}
		new_env->next = NULL;
		append_env_to_list(&head, new_env);
		i++;
	}
	return (head);
}
t_env	*get_env_node(t_env *env_list, const char *name)
{
	size_t	name_len;

	if (!name)
		return (NULL);
	name_len = ft_strlen(name);
	while (env_list)
	{
		if (ft_strncmp(env_list->name, name, name_len) == 0
		&& env_list->name[name_len] == '\0') // second check ensures exact match
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}
char	*variable_name(mem_arena *env_arena, const char *input, int *len)
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
char *expand_string(mem_arena *env_arena, char *input, t_env *env, int exit_status)
{
    int	i;
	int	single_quotes;
	int	double_quotes;
    char *result;
	char	*status;
	char	*var_name;
	int	len;
	t_env *node;

	if (!input || !env_arena)
		return (NULL);
	i = 0;
	single_quotes = 0;
	double_quotes = 0;
	result = arena_strdup(env_arena, "");
	if (!result)
		return (NULL);
    while (input[i])
    {
        handle_quote_flags(input[i], &single_quotes, &double_quotes);
        if (input[i] == '$' && !single_quotes)
        {
            i++;
            if (input[i] == '?')
            {
                status = arena_itoa(env_arena, exit_status);
				if (!status)
					return (NULL);
                result = ar_strjoin(env_arena, result, status);
                i++;
                continue;
            }
            var_name = variable_name(env_arena, &input[i], &len); 
            if (var_name)
            {
                node = get_env_node(env, var_name);
                if (node && node->value)
                    result = ar_strjoin(env_arena, result, node->value);
                else
                    result = ar_strjoin(env_arena, result, "");
				if (!result)
					return (NULL);
                i += len;
                continue;
            }
            result = ar_add_char_to_str(env_arena, result, '$');
			if (!result)
				return (NULL);
            continue;
        }
        result = ar_add_char_to_str(env_arena, result, input[i]);
		if (!result)
			return (NULL);
        i++;
    }
    return (result);
}
char *remove_quotes(mem_arena *env_arena, char *input)
{
	int i;
	int single_quotes;
	int double_quotes;
	char *result;

	if (!input || !env_arena)
		return (NULL);
	i = 0;
    single_quotes = 0;
    double_quotes = 0;
    result = arena_strdup(env_arena, "");
    if (!result)
        return (NULL);
    while (input[i])
    {
        if (input[i] == '\'' && !double_quotes)
        {
            single_quotes = !single_quotes;
            i++;
            continue;
        }
        if (input[i] == '"' && !single_quotes)
        {
            double_quotes = !double_quotes;
            i++;
            continue;
        }
        result = ar_add_char_to_str(env_arena, result, input[i]);
        if (!result)
            return (NULL);
        i++;
    }
    return (result);
}

t_token	*expand_tokens(mem_arena *env_arena, t_token *tokens, t_env *env, int exit_status)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == WORD && current->value)
		{
			current->value = expand_string(env_arena, current->value, env, exit_status);
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
