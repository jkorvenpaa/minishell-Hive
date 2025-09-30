# include "minishell.h"
# include "execution.h"

volatile sig_atomic_t g_sig;

void	sig_handler(int sig)
{
	(void) sig;

	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	init_signals()
{
	struct sigaction sa_int;

	g_sig = 0;
	signal(SIGQUIT, SIG_IGN);
	ft_memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = sig_handler;
	sa_int.sa_flags = SA_RESTART;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
}

void	hd_handler(int sig)
{
	(void) sig;
	g_sig = SIGINT;
	ft_putstr_fd("\n", STDOUT_FILENO);
	//rl_done = 1;
	//ft_putstr_fd("\n", STDOUT_FILENO);
	close(STDIN_FILENO);
	//rl_replace_line("", 1);
	//rl_on_new_line();
	//rl_redisplay();
}

void	heredoc_signals()
{
	struct sigaction sa_hd;

	signal(SIGQUIT, SIG_IGN);
	ft_memset(&sa_hd, 0, sizeof(sa_hd));
	sa_hd.sa_handler = hd_handler;
	sa_hd.sa_flags = 0;
	sigemptyset(&sa_hd.sa_mask);
	sigaction(SIGINT, &sa_hd, NULL);
}

void	ignore_handler(int sig)
{
	(void) sig;
	ft_putstr_fd("\n", STDOUT_FILENO);
}
void	ignore_signals()
{	
	struct sigaction sa_ig;

	signal(SIGQUIT, SIG_IGN);
	ft_memset(&sa_ig, 0, sizeof(sa_ig));
	sa_ig.sa_handler = ignore_handler;
	sa_ig.sa_flags = 0;
	sigemptyset(&sa_ig.sa_mask);
	sigaction(SIGINT, &sa_ig, NULL);
}

void	child_signals()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}