/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:10:40 by nmascaro          #+#    #+#             */
/*   Updated: 2025/09/28 15:06:55 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*	NEEDS TO BE CHECKED/FIXED:

echo hi out1>out2>out3 , should build all files(now builds the last only)

restrict input len? = ft_strlen(input) > ???, throw an error, display prompt?;

env -i. What happens if env is set to null and try to export=crash?!. 

signals

*/


#include "minishell.h"
#include "execution.h"

volatile sig_atomic_t g_sigint;

t_shell	*init_shell(mem_arena *arena, mem_arena *env_arena, char const **envp)
{
	t_shell	*shell;

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

void	sigint_handler(int sig)
{
	(void)sig;
	//g_sigint = false;

		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	g_sigint = true;
	
}

void	exit_shell(t_shell *shell)
{
	int	e;

	rl_clear_history();
	e = shell->exit_status;
	printf("exit\n");
	free_arena(shell->env_arena);
	if (shell->arena)
		free_arena(shell->arena);
	free(shell);
	exit(e);
}

int main(int argc, char **argv, char const **envp)
{
	mem_arena *arena;
	mem_arena *env_arena;
	t_shell	*shell;
	t_command	*command_list;
	t_parser_context data;
	char	*input;

	(void) argv;
	if (argc != 1)
		return (1);
	arena = arena_init();
	env_arena = arena_init();
	shell = init_shell(arena, env_arena, envp);
	g_sigint = false;
	signal(SIGINT, sigint_handler);  // Handle Ctrl+C
    signal(SIGQUIT, SIG_IGN);        // Ignore Ctrl+backlash
	while (1)
	{
		if (g_sigint)
			g_sigint = false;
		input = readline("minishell$ ");
		if (input == NULL)
			exit_shell(shell);
		data = init_parser_context_from_shell(shell);
		command_list = run_parser(input, &data, shell);
		handle_heredoc(shell, command_list);
		if (command_list)
		{
			add_history(input);
			execution(shell, command_list);
			unlink_infile(command_list);
		}
		arena_reset(shell->arena);
		free(input);
	}
	return (0);
}
