/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:56:42 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/09/22 13:59:41 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

void	child_sigint(int sig)
{
	(void)sig;
	g_sigint = false;
}

void	execute_child_command(t_command *command, t_shell *shell)
{
	char	*path;
	char	**env_array;

	if (prepare_files(command) != 0)
		exit(EXIT_FAILURE);
	if (!command->argv || !command->argv[0] || command->argv[0][0] == '\0')
		command_error(command->argv[0]);
	path = find_command_path(command, shell);
	if (!path)
		command_error(command->argv[0]);
	env_array = env_to_array(shell);
	execve(path, command->argv, env_array);
	execve_error(command->argv[0]);
}

void	child_pipe_handler(t_command *command, int pipe_fd, int fd[2])
{
	signal(SIGINT, child_sigint);
	if (pipe_fd != -1)
	{
		if (dup2(pipe_fd, STDIN_FILENO) == -1) // redirect in to open pipe_fd
			perror("dupfailSTDIN");
		close(pipe_fd);
	}
	if (command->next) //pipe is not open but there's next
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1) //redirect out to next pipe
			perror("dupfailSTDOUT");
		close(fd[1]);
	}
}

int	command_loop(t_command *command, t_shell *shell, int pipe_fd)
{
	pid_t	pid;
	int		fd[2];

	if (command->next)
		pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		child_pipe_handler(command, pipe_fd, fd);
		execute_child_command(command, shell);
	}
	else
	{
		command_exit_status(shell, pid);
		if (pipe_fd != -1)
			close(pipe_fd);
		if (command->next)
		{
			close(fd[1]);
			pipe_fd = fd[0];
		}
		else
			pipe_fd = -1;
	}
	return (pipe_fd);
}

void	execution(t_shell *shell, t_command	*command)
{
	static int		pipe_fd = -1;

	while (command)
	{
		if (command->argv)
		{
			if (is_built_in(command) == true && pipe_fd == -1
				&& !command->next && !command->infile && !command->outfile)
				execute_built_in(command, shell);
			else
			{
				pipe_fd = command_loop(command, shell, pipe_fd);
			//	printf("pipe = %d\n", pipe_fd);
			}
		}
		command = command->next;
	}
	return ;
}
