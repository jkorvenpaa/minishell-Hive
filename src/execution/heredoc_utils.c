/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:13:55 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/11 16:57:32 by jkorvenp         ###   ########.fr       */
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
	ft_putstr_fd("\n", STDOUT_FILENO);
	close(STDIN_FILENO);
}

void	heredoc_signals(void)
{
	signal(SIGINT, hd_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	hd_exit_handler(int sig)
{
	(void) sig;
	g_sig = 0;
	
	rl_replace_line("", 0);
	rl_done = 1;
	//rl_on_new_line();
	//rl_redisplay();
}

void	hd_exit_signals(void)
{
	signal(SIGINT, hd_exit_handler);
	signal(SIGQUIT, SIG_IGN);
}
