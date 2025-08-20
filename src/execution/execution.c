/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:56:42 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/20 10:39:29 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	*init_shell(mem_arena *arena, t_shell *shell, t_env *env)
{
	shell = arena_alloc(arena, sizeof(t_shell));
	if (!shell)
		return (NULL); // free arena +?
	shell->exit_status = 0;
	shell->env_list = env;
	shell->arena = arena;
	return (shell);
}
/*
move this to parsing
void	*init_env_list(mem_arena arena)
{
	t_env	*env_node;

	env_node = arena_alloc(arena, sizeof(t_env));
	if (!env_node)
		return (NULL); //free arena +?
	bzero(env_node, sizeof(t_env));
	return (env_node);
}
	*/

void	execution(mem_arena *arena, t_command *command, t_env *env)
{
	t_shell	*shell;

	shell = init_shell(arena, shell, env);
	// if(!shell) ??
	while (command)
	{
		command_validation(command, env, shell);
		command = command->next;
	}
	free_arena(arena);
	return ;
}
