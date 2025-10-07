/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 10:19:48 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/07 14:09:32 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	cmd_list = group_commands(data->arena, tokens);
	return (cmd_list);
}
