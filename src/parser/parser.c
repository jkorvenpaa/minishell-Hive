/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 10:19:48 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/09 10:46:14 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Checks if the value after redirections (except heredoc) is NULL,
 * empty or was split, and throws the error accordingly.
 */
static int	ambiguous_redirection_check(t_token *tokens)
{
	t_token	*target;

	while (tokens)
	{
		if (tokens->type == RED_OUT || tokens->type == RED_IN
			|| tokens->type == APPEND)
		{
			target = tokens->next;
			if (!target || target->value[0] == '\0' || target->was_split)
			{
				ft_putstr_fd("minishell: ambiguous redirect\n", 2);
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}

t_command	*run_parser(char *input, t_parser_context *data, t_shell *shell)
{
	t_token		*tokens;
	t_command	*cmd_list;

	tokens = tokenize_input(data->arena, input);
	if (!tokens)
		return (NULL);
	if (!validate_syntax(tokens, shell))
		return (NULL);
	tokens = exp_toks(shell, tokens);
	tokens = split_expanded_variables(data->env_arena, tokens);
	if (!tokens)
		return (NULL);
	tokens = remove_empty_tokens(tokens);
	if (!ambiguous_redirection_check(tokens))
		return (NULL);
	cmd_list = group_commands(data->arena, tokens);
	return (cmd_list);
}
