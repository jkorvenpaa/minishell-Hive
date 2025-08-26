/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:10:40 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/26 14:28:56 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*init_shell(mem_arena *arena, mem_arena *env_arena, char **envp)
{
	t_shell	*shell;

	shell = arena_alloc(arena, sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->exit_status = 0;
	shell->env_list = init_env_list(env_arena, *envp);
	//shell->history_list = history;
	shell->arena = arena;
	shell->env_arena = env_arena;
	return (shell);
}

int main(int argc, char **argv, char **envp)
{
	mem_arena *arena;
	mem_arena *env_arena;
	t_shell	*shell;
	t_command	*command_list;

	(void)argv;
	if (argc != 1)
		return(1);
	arena = arena_init();
	env_arena = arena_init();
	shell = init_shell(arena, env_arena, envp);
	command_list = run_parser(arena, env_arena);
	execution(shell, command_list);
	//free arenas for env and history only in exit
	return (0);
}