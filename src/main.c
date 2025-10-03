/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:10:40 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/03 14:01:27 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*	NEEDS TO BE CHECKED/FIXED:


directory stuck

signals \n

THINGS TO REMEMBER TO SAY TO JENNI ON MONDAY

Norminette is basically almost done, just some functions with more than 25 and comments in execution that i didn't
want to touch of course. And also need to add headers in some files. And also comment in arena.h about the 
#define (didn't touch it because not sure)
Fixed:
-I changed the getenv function in the find command path to be the getenv_node. Unset wasn't working
-The heredoc temporary files weren't being deleted when normal success, i added in execution.c to track the beginning of the list
To have in mind:
-We have some still reachable memory when running some commands coming from init_shell and arena_init, unsure what
to do with those (is still reachable memory okay to have?)
*/

#include "minishell.h"

t_shell	*init_shell(char const **envp)
{
	t_shell		*shell;
	t_mem_arena	*arena;
	t_mem_arena	*env_arena;

	arena = arena_init();
	env_arena = arena_init();
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->exit_status = 0;
	shell->env_list = init_env_list(env_arena, envp);
	shell->arena = arena;
	shell->env_arena = env_arena;
	shell->expansion.env_arena = env_arena;
	shell->expansion.env = shell->env_list;
	shell->expansion.exit_status = 0;
	shell->fd_in = dup(STDIN_FILENO);
	return (shell);
}

t_parser_context	init_parser_context_from_shell(t_shell *shell)
{
	t_parser_context	data;

	data.arena = shell->arena;
	data.env_arena = shell->env_arena;
	data.env = shell->env_list;
	data.exit_status = shell->exit_status;
	return (data);
}

void	exit_shell(t_shell *shell)
{
	int	e;

	rl_clear_history();
	e = shell->exit_status;
	printf("exit\n");
	close (shell->fd_in);
	free_arena(shell->env_arena);
	if (shell->arena)
		free_arena(shell->arena);
	free(shell);
	exit(e);
}

int	main(int argc, char **argv, char const **envp)
{
	t_shell				*shell;
	t_command			*command_list;
	t_parser_context	data;
	char				*input;

	(void) argv;
	(void) argc;
	shell = init_shell(envp);
	while (1)
	{
		init_signals();
		dup2(shell->fd_in, STDIN_FILENO);
		input = readline("minishell$ ");
		if (input == NULL)
			exit_shell(shell);
		if (ft_strlen(input) < ARG_MAX)
		{
			data = init_parser_context_from_shell(shell);
			command_list = run_parser(input, &data, shell);
			if (command_list && handle_heredoc(shell, command_list) == 0)
			{
				add_history(input);
				execution(shell, command_list);
			}
			arena_reset(shell->arena);
		}
		else
			printf("ARG_MAX exceeded\n");
		free(input);
	}
	return (0);
}
