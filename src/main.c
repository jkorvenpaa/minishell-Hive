/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:10:40 by nmascaro          #+#    #+#             */
/*   Updated: 2025/09/08 11:52:15 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	//shell->history_list = history;
	shell->arena = arena;
	shell->env_arena = env_arena;
	return (shell);
}

void	sigint_handler(int sig)
{
	(void)sig;
	g_sigint = true;
	printf("\n");
	rl_replace_line("", 1);
	rl_on_new_line();
	//rl_redisplay();
}

int main(int argc, char **argv, char const **envp)
{
	mem_arena *arena;
	mem_arena *env_arena;
	t_shell	*shell;
	t_command	*command_list;
	char	*input;

	(void) argv;
	(void)argc;
//	if (argc != 1)
//		return (1);
	arena = arena_init();
	env_arena = arena_init();
	shell = init_shell(arena, env_arena, envp);
	g_sigint = false;
	signal(SIGINT, sigint_handler);   // Handle Ctrl+C
    signal(SIGQUIT, SIG_IGN);        // Ignore Ctrl+backlash
	while (1)
	{
		if (shell->arena)
		{
			free_arena(shell->arena);
			arena = arena_init();
			shell->arena = arena;
		}
		input = readline("minishell$ ");
		if (input == NULL)
			break ;
		command_list = run_parser(input, arena, env_arena, shell->env_list, shell->exit_status);
		//if (command_list->heredoc)
		//	handle_heredoc(argv[1], shell, command_list); //, data, hdoc_quoted);
		if (command_list)
			execution(shell, command_list);
		//free(input);

	}
	//free(input);
	//exit_builtin(shell);
	return (0);
}
