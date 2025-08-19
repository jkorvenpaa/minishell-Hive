/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:43:28 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/19 13:52:27 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * 
 */
void	handle_quote_flags(char c, int *single_quotes, int *double_quotes)
{
	if (c == '\'' && *double_quotes == 0)
		*single_quotes = !(*single_quotes);
	else if (c == '"' && *single_quotes == 0)
		*double_quotes = !(*double_quotes);
}

/**
 * 
 */
static int	handle_unclosed_quotes(t_token *tokens)
{
	int	single_quotes;
	int	double_quotes;
	char	*str;
	int	i;

	single_quotes = 0;
	double_quotes = 0;
	while (tokens)
	{
		i = 0;
		str = tokens->value;
		while (str[i])
		{
			handle_quote_flags(str[i], &single_quotes, &double_quotes);
			i++;
		}
		tokens = tokens->next;
	}
	if (single_quotes || double_quotes)
	{
		printf("syntax error: unclosed quotes\n");
		return (0);
	}
	return (1);
}
/**
 * 
 */
static int	validate_pipes(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (current && current->type == PIPE)
	{
		printf("syntax error near unexpected token `|'\n");
		return (0);
	}
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!current->next || current->next->type == PIPE
			|| current->next->type == REDIRECT_IN || current->next->type == REDIRECT_OUT
			|| current->next->type == APPEND || current->next->type == HEREDOC)
			{
				printf("syntax error near unexpected token `|'\n");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}
/**
 * 
 */
static int	validate_redirections(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == REDIRECT_IN || current->type == REDIRECT_OUT
		|| current->type == APPEND || current->type == HEREDOC)
		{
			if (!current->next || current->next->type != WORD)
			{
				printf("syntax error: expected filename after redirection\n");
				return (0);	
			}
			current = current->next;
		}
		current = current->next;
	}
	return (1);
}
/**
 * 
 */
int	validate_syntax(t_token *tokens)
{
	if (!tokens) //empty input is valid, exits early
		return (1);
	if (!handle_unclosed_quotes(tokens))
		return (0);
	if (!validate_pipes(tokens))
		return (0);
	if (!validate_redirections(tokens))
		return (0);
	return (1);	
}
