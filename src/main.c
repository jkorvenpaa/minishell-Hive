/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:10:40 by nmascaro          #+#    #+#             */
/*   Updated: 2025/09/08 11:07:05 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*init_shell(mem_arena *arena, mem_arena *env_arena, char const **envp)
{
	t_shell	*shell;

	shell = arena_alloc(arena, sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->exit_status = 0;
	shell->env_list = init_env_list(env_arena, envp);
	//shell->history_list = history;
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

int main(int argc, char **argv, char const **envp)
{
	mem_arena *arena;
	mem_arena *env_arena;
	t_shell	*shell;
	t_command	*command_list;
	t_parser_context data;
	char	*input;

	(void)argv;
	(void)argc;
	//if (argc != 1)
	//	return(1);
	arena = arena_init();
	env_arena = arena_init();
	shell = init_shell(arena, env_arena, envp);
	while (1)
	{
		input = readline("minishell$ ");
		if (input == NULL)
			break; //exit_builtin
		data = init_parser_context_from_shell(shell);
		command_list = run_parser(input, &data);
	//	if (command_list == NULL)
			//free(input); // +something else?
		if (command_list)
			execution(shell, command_list);
		free(input);
	}
	//free arenas for env and history only in exit
	return (0);
}
