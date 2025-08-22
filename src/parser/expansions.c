/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:24:32 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/22 09:23:53 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

char	*arena_strdup(mem_arena arena, const char *str) //we need this in arena.c and need to call this version everywhere where currently is ft_strdup
{
	char	*dest;
	size_t	i;

	if (!str)
		return (NULL);
	dest = arena_alloc(arena, ft_strlen(str) + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (str[i])
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ar_substr(mem_arena arena, char const *s, unsigned int st, size_t len) //maybe also needed in arena.c
{
	size_t	i;
	size_t	j;
	size_t	slen;
	char	*substr;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (st >= slen)
		return (arena_strdup(arena, ""));
	if (len > slen - st)
		len = slen - st;
	substr = arena_alloc(arena, len + 1);
	if (!substr)
		return (NULL);
	i = st;
	j = 0;
	while (s[i] && j < len)
	{
		substr[j] = s[i];
		i++;
		j++;
	}
	substr[j] = '\0';
	return (substr);
}

char	*ar_strjoin(mem_arena arena, char const *s1, char const *s2) //in arena .c too probably
{
	char	*joinedstr;
	size_t	i;
	size_t	j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (arena_strdup(arena, s2)); //return a copy for safety so we dont modify the original environment variable
	if (!s2)
		return (arena_strdup(arena, s1));
	joinedstr = arena_alloc(arena, ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!joinedstr)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
		joinedstr[j++] = s1[i++];
	i = 0;
	while (s2[i])
		joinedstr[j++] = s2[i++];
	joinedstr[j] = '\0';
	return (joinedstr);
}

char	*ar_add_char_to_str(mem_arena arena, const char *s, char c)
{
	size_t	len;
	char	*result;

	if (!s)
		len = 0;
	else
		len = ft_strlen(s);
	result = arena_alloc(arena, len + 2);
	if (!result)
		return (NULL);
	if (s)
		ft_memcpy(result, s, len);
	result[len] = c;
	result[len + 1] = '\0';
	return (result);
}

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

t_env	*init_env_list(mem_arena arena, char **envp)
{
	int	i;
	t_env *head;
	t_env *new_env;
	char *equal_sign;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		new_env = arena_alloc(arena, sizeof(t_env));
		if (!new_env)
			return (NULL);
		ft_bzero(new_env, sizeof(t_env)); //zero all fields of t_env
		equal_sign = ft_strchr(envp[i], '=');  //search for = and sets pointer there
		if (equal_sign)
		{
			new_env->name = ar_substr(arena, envp[i], 0, equal_sign - envp[i]); //before = (p.ex: USER) copies before (but its always set to before the =))
			new_env->value = arena_strdup(arena, equal_sign + 1); // after = (p.ex.: guest) copies everything after equal sign
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
char	*variable_name(mem_arena arena, const char *input, int *len)
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
	return (ar_substr(arena, input, 0, i));
}
char *expand_string(mem_arena arena, char *input, t_env *env, int exit_status)
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
	result = arena_strdup(arena, "");
    while (input[i])
    {
        handle_quote_flags(input[i], &single_quotes, &double_quotes);
        if (input[i] == '$' && !single_quotes)
        {
            i++;
            if (input[i] == '?')
            {
                status = ft_itoa(exit_status);
                result = ar_strjoin(arena, result, status);
                free(status);
                i++;
                continue;
            }
            var_name = variable_name(arena, &input[i], &len); 
            if (var_name)
            {
                node = get_env_node(env, var_name);
                if (node && node->value)
                    result = ar_strjoin(arena, result, node->value);
                else
                    result = ar_strjoin(arena, result, "");
                i += len;
                continue;
            }
            result = ar_strjoin_char(arena, result, '$');
            continue;
        }
        result = ar_strjoin_char(arena, result, input[i]);
        i++;
    }
    return (result);
}

t_token	expand_tokens(mem_arena arena, t_token *tokens, t_env *env, int exit_status)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == WORD && current->value)
			current->value = expand_string(arena, current->value, env, exit_status);
		current = current->next;
	}
	return (tokens);
}
