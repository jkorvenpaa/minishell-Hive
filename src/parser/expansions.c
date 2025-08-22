/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:24:32 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/22 12:37:47 by jkorvenp         ###   ########.fr       */
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

t_env	*init_env_list(char **envp)
{
	int	i;
	t_env *head;
	t_env *new_env;
	char *equal_sign;
	//mem_arena *env_arena;

	//env_arena = arena_init();
	i = 0;
	head = NULL;
	while (envp[i])
	{
		new_env = arena_alloc(env_arena, sizeof(t_env));
		if (!new_env)
			return (NULL);
		ft_bzero(new_env, sizeof(t_env)); //zero all fields of t_env
		equal_sign = ft_strchr(envp[i], '=');  //search for = and sets pointer there
		if (equal_sign)
		{
			new_env->name = ar_substr(env_arena, envp[i], 0, equal_sign - envp[i]); //before = (p.ex: USER) copies before (but its always set to before the =))
			new_env->value = arena_strdup(env_arena, equal_sign + 1); // after = (p.ex.: guest) copies everything after equal sign
		}
		new_env->next = NULL;
		append_env_to_list(&head, new_env);
		i++;
	}
	return (head);
}
t_env	*get_env_node(t_env *env_list, const char *name) //USEFUL IN EXECUTION TOO (for like export, unset, env because it loops through our t_env list and looks for match and returns pointer to the node that contains that)
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
	if (!ft_isalpha(input[i]) && input[i] !+ '_')
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

	i = 0;
	single_quotes = 0;
	double_quotes = 0;
	result = arena_strdup(env_arena, "");
    while (input[i])
    {
        handle_quote_flags(input[i], &single_quotes, &double_quotes);
        if (input[i] == '$' && !single_quotes)
        {
            i++;
            if (input[i] == '?')
            {
                status = ft_itoa(exit_status);
                result = ar_strjoin(env_arena, result, status);
                free(status);
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
                i += len;
                continue;
            }
            result = ar_strjoin_char(env_arena, result, '$');
            continue;
        }
        result = ar_strjoin_char(env_arena, result, input[i]);
        i++;
    }
    return (result);
}

t_token	expand_tokens(mem_arena *env_arena, t_token *tokens, t_env *env, int exit_status)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == WORD && current->value)
			current->value = expand_string(env_arena, current->value, env, exit_status);
		current = current->next;
	}
	return (tokens);
}
