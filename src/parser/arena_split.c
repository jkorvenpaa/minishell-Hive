/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:55:03 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/10/03 11:41:09 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*Allocates memory and returns an array of strings, using
the character ’c’ as a delimiter. The array is null-terminated*/

#include "minishell.h"

static size_t	ft_count(char const *s, char c)
{
	size_t	start;
	size_t	wcount;

	start = 0;
	wcount = 0;
	while (s[start])
	{
		while (s[start] == c)
			start++;
		if (s[start] == '\0')
			break ;
		while (s[start] != c && s[start])
			start++;
		wcount++;
	}
	return (wcount);
}

static void	*ft_findword(t_shell *shell, char **array, char const *s, char c)
{
	size_t	start;
	size_t	end;
	size_t	i;

	start = 0;
	end = 0;
	i = 0;
	while (s[start])
	{
		while (s[start] == c)
			start++;
		if (s[start] == '\0')
			break ;
		end = start;
		while (s[end] != c && s[end])
			end++;
		array[i] = a_substr(shell->arena, s, start, end - start);
		if (!array[i])
			return (NULL);
		i++;
		start = end;
	}
	array[i] = NULL;
	return (array);
}

char	**arena_split(t_shell *shell, char const *s, char c)
{
	char	**array;
	int		count;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	count = ft_count(s, c);
	array = arena_alloc(shell->arena, sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	ft_findword(shell, array, s, c);
	return (array);
}
