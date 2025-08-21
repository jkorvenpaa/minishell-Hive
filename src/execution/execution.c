/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:56:42 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/21 17:10:19 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	*init_shell(mem_arena *arena, t_shell *shell, t_env *env)
{
	shell = arena_alloc(arena, sizeof(t_shell));
	if (!shell)
		return (NULL); // free arena +?
	shell->exit_status = 0;
	shell->env_list = init_env_list(arena, env);
	//shell->history_list = history;
	shell->arena = arena;
	return (shell);
}
void	execution(mem_arena *arena, t_command *command, t_env *env)
{
	t_shell	*shell;
	//init_history
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
/*
init history, shell first time or in the main,
consider which arena shell should use.
loop user inputs;
{
	command validation, action, history.
	after executing command:
		Free arenas used for tokens, commands.
	Loop back to prompt.
}
free arenas for env and history only in exit

*/
