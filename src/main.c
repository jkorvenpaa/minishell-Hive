/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:10:40 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/14 16:41:35 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	shell->working_dir = NULL;
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

static void	shell_loop(t_shell *shell)
{
	char				*input;
	t_parser_context	data;
	t_command			*command_list;

	input = readline("minishell$ ");
	if (!input)
		exit_shell(shell);
	if (g_sig == SIGINT)
	{
		shell->exit_status = 130;
		g_sig = 0;
	}
	if (ft_strlen(input) < ARG_MAX)
	{
		data = init_parser_context_from_shell(shell);
		command_list = run_parser(input, &data, shell);
		add_history(input);
		if (command_list && handle_heredoc(shell, command_list) == 0)
		{
			execution(shell, command_list);
		}
		unlink_infile(command_list);
		arena_reset(shell->arena);
	}
	else
		printf("ARG_MAX exceeded\n");
	free(input);
}

int	main(int argc, char **argv, char const **envp)
{
	t_shell				*shell;

	(void) argv;
	(void) argc;
	shell = init_shell(envp);
	while (1)
	{
		init_signals();
		dup2(shell->fd_in, STDIN_FILENO);
		shell_loop(shell);
	}
	return (0);
}
