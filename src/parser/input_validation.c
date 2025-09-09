/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:43:28 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/09/09 11:08:56 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Iterates through all tokens and toggles quote states accordingly.
 * If any quotes remain unclosed by the end, an error message is printed.
 * Returns 1 if all quotes are closed, 0 otherwise.
 */
static int	handle_unclosed_quotes(t_token *tokens)
{
	int		single_quotes;
	int		double_quotes;
	char	*str;
	int		i;

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
 * Validates correct pipe usage.
 * Ensures that:
 * - Input doesn't start or end with a pipe.
 * - Pipes aren't followed by another pipe or a redirection operator.
 * Returns 1 if pipe usage is valid, 0 otherwise.
 */
static int	validate_pipes(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	if (curr && curr->type == PIPE)
	{
		printf("syntax error near unexpected token `|'\n");
		return (0);
	}
	while (curr)
	{
		if (curr->type == PIPE)
		{
			if (!curr->next || curr->next->type == PIPE
				|| curr->next->type == RED_IN || curr->next->type == RED_OUT
				|| curr->next->type == APPEND || curr->next->type == HEREDOC)
			{
				printf("syntax error near unexpected token `|'\n");
				return (0);
			}
		}
		curr = curr->next;
	}
	return (1);
}

/**
 * Validates correct redirection operator usage.
 * Ensures every redirection token is followed by a valid WORD
 * token (filename).
 * Returns 1 if all redirections are valid, 0 otherwise.
 */
static int	validate_redirections(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == RED_IN || current->type == RED_OUT
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
 * Validates that no more than 16 HEREDOC are in the input.
 * Returns 1 if limit isn't exceeded, exits the program otherwise.
 */
static int	validate_heredoc_count(t_token *tokens)
{
	t_token	*current;
	int		heredoc_count;

	current = tokens;
	heredoc_count = 0;
	while (current)
	{
		if (current->type == HEREDOC)
			heredoc_count++;
		current = current->next;
	}
	if (heredoc_count > 16)
	{
		printf("minishell: maximum here-document count exceeded\n");
		exit(2); //bash exits with that code in this case (should we use exit function though?)
	}
	return (1);
}

/**
 * Main function that validates syntax on the token list.
 * Ensures that:
 * - Quotes are closed.
 * - Pipes are used correctly.
 * - Redirections are valid.
 * Takes into account that empty input (pressing Enter) is valid.
 * Returns 1 if syntax is valid, 0 otherwise.
 */
int	validate_syntax(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!handle_unclosed_quotes(tokens))
		return (0);
	if (!validate_pipes(tokens))
		return (0);
	if (!validate_redirections(tokens))
		return (0);
	validate_heredoc_count(tokens);
	return (1);
}
