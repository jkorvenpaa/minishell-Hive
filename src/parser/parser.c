/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 10:19:48 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/15 15:08:50 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static void	print_tokens(t_token *head) // for testing only!!!!
{
    while (head)
    {
        printf("Token: %s Type: %d\n", head->value, head->type);
        head = head->next;
    }
}
*/
static void free_command_list(t_command *head) // not sure if i need it since we have arena (i'd  need to but it in grouping command utils)
{
    t_command *tmp;

    while (head)
    {
        tmp = head->next;
        free(head);
        head = tmp;
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
		cmd_list = cmd_list->next;
	}
}

void	run_parser(void) //"main" for parsing part
{
	char	*input;
	t_token	*tokens;
	t_command	*cmd_list;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input) //When readline() returns NULL (we press ctrl + D)
		{
			printf("exit\n");
			break;
		}
		if (ft_strncmp(input, "exit", 5) == 0)
		{
			free(input);
			break;
		}
		tokens = tokenize_input(input);
		if (!tokens)
		{
			printf("Error: failed to parse input\n");
			free(input);
		}
		else
		{
			cmd_list = group_commands(tokens);
			// print_tokens(tokens); // here for testing only!!!
			print_commands(cmd_list); // here for testing only!!
			free_token_list(tokens);
			free_command_list(cmd_list);
			free(input);
		}
	}
}