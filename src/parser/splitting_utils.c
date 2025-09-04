



#include "minishell.h"

/**
 * Decides if a token should be split into multiple words.
 * It needs splitting if it's unquoted and contains at least one whitespace char.
 * Returns 1 if token needs splitting, 0 otherwise.
 */
int	needs_splitting(t_token *token)
{
	char	*str;
	int	i;

	if (!token || !token->value || token->was_quoted)
		return (0);
	str = token->value;
	i = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
			return (1);
		i++;
	}
	return (0);
}

/**
 * Finds the next word in a string starting from the given index.
 * A word is a sequence of non-space characters.
 * Returns 1 if a word was found, 0 if no more words left.
 */
int	get_next_word(char *str, int *start, int *end)
{
	int	i;

	i = *start;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (!str[i])
		return (0);
	*start = i;
	while (str[i] && !ft_isspace(str[i]))
		i++;
	*end = i;
	return (1);
}

int	is_only_spaces(const char *str)
{
	int	i;
	i = 0;

	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}