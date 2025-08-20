/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:24:32 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/20 15:43:39 by nmascaro         ###   ########.fr       */
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
		new_env->expanded = 0;
		new_env->next = head;
		head = new_env;
		i++;
	}
	return (head);
}
t_env	*get_env_node(t_env *env_ist, cons char *name)
{
	size_t	name_len;

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
t_token	expand_tokens(t_token *tokens, t_env *env)
{
	t_token	*current;

	current = tokens;
	if (current->type == WORD)
	{
		
	}
	current = current->next;
}
