/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:10:40 by nmascaro          #+#    #+#             */
/*   Updated: 2025/09/27 17:37:22 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
exit hello 555, exit 55 5
home, unsethoome, cd, change from getenv
signal handler reset after heredoc.
echo hi out1>out2>out3
export updatee the value correctly THIS IS NOT FINDING new = get_env_node(shell->env_list, cmd->argv[i]);
restrict input len = ft_strlen(input) > ???, throw an error, display prompt?;


1. env -i. What happens if env is set to null and try to export, crash. 
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
			break; //exit_builtin
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
	rl_clear_history();
	exit_builtin(shell);
	return (0);
}
