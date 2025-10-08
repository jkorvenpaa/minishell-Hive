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
	//ft_putstr_fd("\n", STDOUT_FILENO);
	//rl_done = 1;
	//ft_putstr_fd("\n", STDOUT_FILENO);
	close(STDIN_FILENO);
	//rl_replace_line("", 1);
	//rl_on_new_line();
	//rl_redisplay();
}

void	heredoc_signals(void)
{
	struct sigaction	sa_hd;

	signal(SIGQUIT, SIG_IGN);
	ft_memset(&sa_hd, 0, sizeof(sa_hd));
	sa_hd.sa_handler = hd_handler;
	sa_hd.sa_flags = 0;
	sigemptyset(&sa_hd.sa_mask);
	sigaction(SIGINT, &sa_hd, NULL);
}

void	hd_exit_handler(int sig)
{
	(void) sig;

	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	hd_exit_signals(void)
{
	struct sigaction	sa_hd_exit;

	signal(SIGQUIT, SIG_IGN);
	ft_memset(&sa_hd_exit, 0, sizeof(sa_hd_exit));
	sa_hd_exit.sa_handler = hd_exit_handler;
	sa_hd_exit.sa_flags = 0;
	sigemptyset(&sa_hd_exit.sa_mask);
	sigaction(SIGINT, &sa_hd_exit, NULL);
}
