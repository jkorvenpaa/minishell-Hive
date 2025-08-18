/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:43:28 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/18 16:22:49 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quote_flags(char c, int *single_quotes, int *double_quotes) //can't be static
{
	if (c == '\'' && *double_quotes == 0)
		*single_quotes = !(*single_quotes);
	else if (c == '"' && *single_quotes == 0)
		*double_quotes = !(*double_quotes);
}
int	handle_unclosed_quotes(t_token *tokens) //maybe static
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

int	validate_pipes(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;

	current = tokens;
	prev = NULL;
	if (current && current->type == PIPE)
	{
		printf("syntax error near unexpected token `|'\n");
		return (0);
	}
	while (current)
	{
		if (current->type == PIPE)
		{
			if (prev && prev->type == PIPE)
			{
				printf("syntax error near unexpected token `|'\n");
				return (0);
			}
			if (!current->next)
			{
				printf("syntax error near unexpected token `|'\n");
				return (0);
			}
			if (current->next && (current->next->type == PIPE || current->next->type == REDIRECT_IN
			|| current->next->type  == REDIRECT_OUT || current->next->type == APPEND
			|| current->next->type == HEREDOC))
			{
				printf("syntax error near unexpected token `|'\n");
				return (0);
			}
		}
		prev = current;
		current = current->next;
	}
	return (1);
}
int	validate_redirections(t_token *tokens)
{

}
int	validate_empty_commands(t_token *tokens)
{

}
int	validate_syntax(t_token *tokens)
{
	if (!handle_unclosed_quotes(tokens))
		return (0);
	if (!tokens) //empty input is valid
		return (1);
	if (!validate_pipes(tokens))
		return (0);
	if (!validate_redirections(tokens))
		return (0);
	if (!validate_empty_commands)
		return (0);
	return (1);	
}
