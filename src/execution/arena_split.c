# include "minishell.h"
# include "execution.h"

static size_t	ft_count_words(char const *s, char c)
{
	size_t	counter;
	int		in_word;

	counter = 0;
	in_word = 0;
	while (*s)
	{
		if (*s == c)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			counter++;
		}
		s++;
	}
	return (counter);
}

static size_t	ft_wordlen(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}
/*
static char	**ft_free(char **array)
{
	size_t	i;

	i = 0;
	if (array == NULL)
		return (NULL);
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}
	*/

static char	**ft_words(mem_arena *arena, char **result, size_t words, char const *s, char c)
{
	size_t	len_word;
	size_t	i;

	i = 0;
	while (*s && i < words)
	{
		if (*s == c)
			s++;
		else
		{
			len_word = ft_wordlen(s, c);
			result[i] = ar_substr(arena, s, 0, len_word);
			if (result[i] == NULL)
				return (NULL);
			s = s + len_word;
			i++;
		}
	}
	result[i] = NULL;
	return (result);
}

char	**ar_split(mem_arena *arena, char const *s, char c)
{
	size_t	words;
	char	**result;

	if (!s)
		return (NULL);
	words = ft_count_words (s, c);
	result = (char **) arena_alloc(arena, ((words + 1) * sizeof(*result)));
	if (!result)
		return (NULL);
	return (ft_words(arena, result, words, s, c));
}
