/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:10:40 by nmascaro          #+#    #+#             */
/*   Updated: 2025/09/09 14:25:08 by nmascaro         ###   ########.fr       */
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
	//shell->history_list = history; // seems we actually dont need this and we dont need the history struct at all (readline already stores commands for us)
	shell->arena = arena;
	shell->env_arena = env_arena;
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
	t_parser_context data;
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
			break; //exit_builtin
		data = init_parser_context_from_shell(shell);
		command_list = run_parser(input, &data);
		if (command_list)
		{
			add_history(input);
			execution(shell, command_list);
		}
		arena_reset(shell->arena);
		//if (command_list->heredoc)
		//	handle_heredoc(argv[1], shell, command_list); //, data, hdoc_quoted);
		//free(input);
	}
	rl_clear_history();
	//free(input);
	exit_builtin(shell);
	return (0);
}
