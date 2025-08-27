/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 10:19:48 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/27 15:56:02 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_tokens(t_token *head) // for testing only!!!!
{
    while (head)
    {
        printf("Token: %s Type: %d\n", head->value, head->type);
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

t_command	*run_parser(char *input, mem_arena *arena, mem_arena *env_arena, t_env *env, int exit_status) //"main" for parsing part
{
	//char	*input;
	t_token	*tokens;
	t_command	*cmd_list;

	//while (1)
	//{
		/*input = readline("minishell$ ");
		if (!input) //When readline() returns NULL (we press ctrl + D)
		{
			printf("exit\n");
			break;
		}
		if (ft_strncmp(input, "exit", 5) == 0)
		{
			free(input);
			break;
		}*/
		tokens = tokenize_input(arena, input);
		if (!tokens)
		{
			return (NULL);
			//free(input);
			//continue;
		}
		if (!validate_syntax(tokens))
		{
			return (NULL);
			//free(input);
			//continue;
		}
		tokens = expand_tokens(env_arena, tokens, env, exit_status);
		cmd_list = group_commands(arena, tokens);
		print_tokens(tokens); // here for testing only!!!
		print_commands(cmd_list); // here for testing only!!
		free(input);
	//}
	return (cmd_list);
}
