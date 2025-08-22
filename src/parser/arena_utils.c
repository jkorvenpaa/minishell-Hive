
#include "minishell.h"

char	*arena_strdup(mem_arena *arena, const char *str) //we need this in arena.c and need to call this version everywhere where currently is ft_strdup
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

char	*ar_substr(mem_arena *arena, char const *s, unsigned int st, size_t len) //maybe also needed in arena.c
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

char	*ar_strjoin(mem_arena *arena, char const *s1, char const *s2) //in arena .c too probably
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