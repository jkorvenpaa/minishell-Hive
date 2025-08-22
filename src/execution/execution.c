/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:56:42 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/22 13:24:37 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	execution(t_shell *shell, t_command	*command_list)
{
	//init_history
	// if(!shell) ??
	while (command_list)
	{
		command_validation(command_list, shell);
		command_list = command_list->next;
	}
	//free_arena(shell->arena);
	return ;
}
/*
loop user inputs;
{
	command validation, action, history.
	after executing command:
		Free arenas used for tokens, commands.
	Loop back to prompt.
}
free arenas for env and history only in exit

*/
