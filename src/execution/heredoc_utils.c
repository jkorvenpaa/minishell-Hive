#include "minishell.h"

//remove heredoc files after command is executed
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

void	heredoc_signals(void)
{
	signal(SIGINT, hd_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	hd_exit_handler(int sig)
{
	(void) sig;
	g_sig = 0;
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	hd_exit_signals(void)
{
	signal(SIGINT, hd_exit_handler);
	signal(SIGQUIT, SIG_IGN);
}
