/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:13:55 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/16 11:20:08 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//remove heredoc tempfiles after command is executed or SIGINT
void	unlink_infile(t_command *command)
{
	while (command)
	{
		if (command->infile
			&& ft_strncmp(command->infile, "hd_temp_file", 12) == 0)
			unlink(command->infile);
		command = command->next;
	}
}

void	hd_handler(int sig)
{
	(void) sig;
	g_sig = SIGINT;
	close(STDIN_FILENO);
}

int	event_handler(void)
{
	if (g_sig == SIGINT)
		rl_done = 1;
	return (0);
}

void	heredoc_signals(void)
{
	rl_event_hook = event_handler;
	signal(SIGINT, hd_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_exit(t_shell *shell)
{
	shell->exit_status = 130;
	g_sig = 0;
}
