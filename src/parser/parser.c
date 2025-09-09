/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 10:19:48 by nmascaro          #+#    #+#             */
/*   Updated: 2025/09/09 11:11:00 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_tokens(t_token *head) // for testing only!!!!
{
	while (head)
	{
		printf("Token: %s | Type: %d | Was quoted %d\n", head->value, head->type, head->was_quoted);
		head = head->next;
	}
}

static void	print_commands(t_command *cmd_list) // for testing only!!!
{
	while (cmd_list)
	{
		int	i;
		i = 0;
		printf("COMMAND:\n");
		if (cmd_list->argv)
		{
			while (cmd_list->argv[i])
			{
				printf("Arg[%d]: %s\n", i, cmd_list->argv[i]);
				i++;
			}
		}
		else
			printf("Arg[]: NULL\n");
		if (cmd_list->infile)
			printf("Infile: %s\n", cmd_list->infile);
		if (cmd_list->outfile)
		{
			printf("Outfile: %s", cmd_list->outfile);
			if (cmd_list->append)
				printf("(append)");
			printf("\n");
		}
		if (cmd_list->heredoc)
			printf("Heredoc: %s\n", cmd_list->heredoc);
		printf("Heredoc was quoted: %d\n", cmd_list->heredoc_quoted);
		cmd_list = cmd_list->next;
	}
}

t_command	*run_parser(char *input, t_parser_context *data)
{
	t_token		*tokens;
	t_command	*cmd_list;

	tokens = tokenize_input(data->arena, input);
	print_tokens(tokens); // here for testing only!!!
	if (!tokens)
		return (NULL);
	if (!validate_syntax(tokens))
		return (NULL);
	tokens = exp_toks(data->env_arena, tokens, data->env, data->exit_status);
	printf("--------AFTER EXPANSION------\n"); // here for testing only!!!
	print_tokens(tokens); // here for testing only!!!
	tokens = split_expanded_variables(data->arena, tokens);
	printf("--------AFTER SPLITTING------\n"); // here for testing only!!!
	print_tokens(tokens); // here for testing only!!!
	if (!tokens)
		return (NULL);
	tokens = remove_empty_tokens(tokens);
	printf("--------AFTER REMOVING EMPTY------\n"); // here for testing only!!!
	print_tokens(tokens); // here for testing only!!!
	cmd_list = group_commands(data->arena, tokens);
	printf("--------AFTER FINAL COMMANDS------\n"); // here for testing only!!!
	print_commands(cmd_list); // here for testing only!!
	return (cmd_list);
}
