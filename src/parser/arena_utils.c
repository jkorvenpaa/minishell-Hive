/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:50:07 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/01 13:57:18 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

char	*arena_strdup(t_mem_arena *arena, const char *str)
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

char	*ar_substr(t_mem_arena *arena, char const *s, unsigned int st, size_t len)
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

char	*ar_strjoin(t_mem_arena *arena, char const *s1, char const *s2)
{
	char	*joinedstr;
	size_t	i;
	size_t	j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (arena_strdup(arena, s2));
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

char	*arena_itoa(t_mem_arena *env_arena, int n)
{
	size_t	len;
	char	*result;
	long	num;

	num = n;
	len = ft_count_digits(n);
	result = arena_alloc(env_arena, sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	result[len] = '\0';
	if (num == 0)
		result[0] = '0';
	else if (num < 0)
	{
		result[0] = '-';
		num = -num;
	}
	while (num > 0)
	{
		result[--len] = (num % 10) + '0';
		num /= 10;
	}
	return (result);
}
