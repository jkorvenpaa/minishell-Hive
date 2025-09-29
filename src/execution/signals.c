# include "minishell.h"
# include "execution.h"

volatile sig_atomic_t g_sig;

void	sig_handler(int sig, siginfo_t *info, void *context)
{
	(void) sig;
	(void) info;
	(void) context;

	if (isatty(STDIN_FILENO))
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

//void	hd_sig_handler(int sig, siginfo_t *info, void *context)

void	init_signals()
{
	g_sig = 0;
	struct sigaction sa_int;
	struct sigaction sa_q;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_sigaction = sig_handler;
	sa_int.sa_flags = SA_RESTART;
	//sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	ft_memset(&sa_q, 0, sizeof(sa_q));
    sa_q.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa_q, NULL);
}

void	heredoc_signals()
{
	struct sigaction sa_hd;

	ft_memset(&sa_hd, 0, sizeof(sa_hd));
	sa_hd.sa_sigaction = sig_handler;
	sa_hd.sa_flags = 0;
	//sigemptyset(&sa_hd.sa_mask);
	sigaction(SIGINT, &sa_hd, NULL);
}
void	ignore()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	child_signals()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	/*
	struct sigaction sa_c;
	struct sigaction sa_q;

	//g_sig = 1;
	ft_memset(&sa_c, 0, sizeof(sa_c));
	sa_c.sa_handler = SIG_DFL;
	sa_c.sa_flags = 0;
	//sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa_c, NULL);
	//ft_putstr_fd("child sigsgisgigigi\n", STDOUT_FILENO);
	ft_memset(&sa_q, 0, sizeof(sa_q));
    sa_q.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa_q, NULL);
	

*/
	//signal(SIGINT, SIG_DFL);

}