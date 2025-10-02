#include "minishell.h"
#include "execution.h"

void	command_error(char *command_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command_name, 2);
	ft_putendl_fd(": command not found", 2);
	exit(127);
}

void	execve_error(char *command_name)
{
	int	len;

	len = ft_strlen(command_name);
	ft_putstr_fd("minishell: ", 2);
	if (len > 0 && command_name[len - 1] == '/') // special case to get the exact same message as bash when it's directory, in linux errno sets it automatically to EACESS (and we would get wrong message)
	{
		ft_putstr_fd(command_name, 2);
		ft_putendl_fd(": is a directory", 2);
		exit(126);
	}
	perror(command_name);
	if (errno == EACCES || errno == ENOEXEC) // execve failed because the file exists but can't be executed or it's not valid executable format
		exit(126);
	if (errno == ENOENT) // execve failed because the file doesn't exist
		exit(127);
	exit(1);
}

int	cmd_count(t_command *command)
{
	int	i;

	i = 0;
	while (command)
	{
		command = command->next;
		i++;
	}
	return (i);
}

void	command_exit_status(t_shell *shell, pid_t pid)
{
	int	child_status;
	int	sig;

	waitpid(pid, &child_status, 0);
	if (WIFEXITED(child_status))
		shell->exit_status = WEXITSTATUS(child_status);
	else if (WIFSIGNALED(child_status))
	{
		sig = WTERMSIG(child_status);
		shell->exit_status = sig + 128;
		g_sig = 1;
	}
}

void	wait_kids(t_shell *shell, int *pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		ignore_signals();
		command_exit_status(shell, pids[i]);
		i++;
	}
}
