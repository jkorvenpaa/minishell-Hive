/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:12:54 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/16 15:25:41 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// exits child if not a valid command
void	command_error(t_shell *shell, char *command_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command_name, 2);
	ft_putendl_fd(": command not found", 2);
	clean_child(shell);
	exit(127);
}

//sets exit codes if execve fails
//ENOEXEC the file exists but can't be executed
//ENOENT the file doesn't exist
void	execve_error(char *command_name, t_shell *shell)
{
	int	len;

	len = ft_strlen(command_name);
	ft_putstr_fd("minishell: ", 2);
	if (len > 0 && command_name[len - 1] == '/')
	{
		ft_putstr_fd(command_name, 2);
		ft_putendl_fd(": is a directory", 2);
		clean_child(shell);
		exit(126);
	}
	perror(command_name);
	clean_child(shell);
	if (errno == EACCES || errno == ENOEXEC)
		exit(126);
	if (errno == ENOENT)
		exit(127);
	exit(1);
}

//count of commands
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

// waits and fetches exit_status
void	command_exit_status(t_shell *shell, pid_t last_pid)
{
	int	child_status;
	int	sig;
	int	wpid;

	sig = 0;
	wpid = waitpid(-1, &child_status, 0);
	if (WIFEXITED(child_status) && wpid == last_pid)
		shell->exit_status = WEXITSTATUS(child_status);
	else if (WIFSIGNALED(child_status))
	{
		sig = WTERMSIG(child_status);
		if (wpid == last_pid)
			shell->exit_status = sig + 128;
	}
}

// loop to wait kids
void	wait_kids(t_shell *shell, int *pids, int count, t_command *command)
{
	int	last_pid;
	int	i;

	last_pid = pids[count - 1];
	i = 0;
	while (i < count)
	{
		command_exit_status(shell, last_pid);
		i++;
	}
	unlink_infile(command);
}
