/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:16:46 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/07 16:52:38 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_separator_symbol(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}
static int	add_char_to_buffer(char **buffer, char c)
{
	char	temp[2];
	char	*joined;

	temp[0] = c;
	temp[1] = '\0';
	if (!*buffer)
	{
		*buffer = ft_strdup(temp);
		if (!*buffer)
			return (0);
	}
	else
	{
		joined = ft_strjoin(*buffer, temp);
		if (!joined)
			return (0);
		free(*buffer);
		*buffer = joined;
	}
	return (1);
}

static int	save_buffer_to_list(t_token **list, char **buffer)
{
	t_token	*new;

	if (*buffer)
	{
		new = create_token_node(*buffer);
		if (!new)
		{
			free_token_list(*list);
			*list = NULL;
			return (0);
		}
		append_token_to_list(list, new);
		free(*buffer);
		*buffer = NULL;
	}
	return (1);
}

static int	add_special_token_to_list(t_token **list, char symbol)
{
	char	special[2];
	t_token	*new;

	special[0] = symbol;
	special[1] = '\0';
	new = create_token_node(special);
	if (!new)
	{
		free_token_list(*list);
		*list = NULL;
		return (0);
	}
	append_token_to_list(list, new);
	return (1);
}

t_token	*tokenize_input(char *input)
{
	int	i;
	char	*current_token;
	t_token	*token_list;
	int	single_quote;
	int	double_quote;

	i = 0;
	current_token = NULL;
	token_list = NULL;
	single_quote = 0;
	double_quote = 0;
	while (input[i])
	{
		update_quote_flags(input[i], &single_quote, &double_quote);
		if (is_token_boundary(input[i], single_quote, double_quote))
		{
			if (!save_buffer_to_list(&token_list, &current_token))
				return (NULL);
			if (is_separator_symbol(input[i]))
			{
				if (!add_special_token_to_list(&token_list, input[i]))
					return (NULL);
			}
		}
		else if (input[i] != ' ' || single_quote || double_quote)
		{
			if (!add_char_to_buffer(&current_token, input[i]))
			{
				free_token_list(token_list);
				if (current_token)
					free(current_token);
				return (NULL);
			}
		}
		i++;
	}
	if (!save_buffer_to_list(&token_list, &current_token))
		return (NULL);
	return (token_list);
}
