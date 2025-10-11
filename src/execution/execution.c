/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:56:42 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/10/11 15:52:19 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_child(t_shell *shell)
{
	close (shell->fd_in);
	free_arena(shell->env_arena);
	if (shell->arena)
		free_arena(shell->arena);
	free(shell);
}

// redirects input/output if infile and outfiles are present
// if built_in, executes built_ins and exits child
// execve for external commands
static void	execute_child_command(t_command *command, t_shell *shell)
{
	char	*path;
	char	**env_array;

	if (prepare_files(command) != 0)
	{
		close(shell->fd_in);
		clean_child(shell);
		exit(EXIT_FAILURE);
	}
	if (!command->argv || !command->argv[0] || command->argv[0][0] == '\0')
		command_error(shell, command->argv[0]);
	if (is_built_in(command) == true)
	{
		execute_built_in(command, shell);
		clean_child(shell);
		exit(EXIT_SUCCESS);
	}
	path = find_command_path(command, shell);
	if (!path)
		command_error(shell, command->argv[0]);
	close(shell->fd_in);
	env_array = env_to_array(shell);
	execve(path, command->argv, env_array);
	execve_error(command->argv[0]);
}

// sets child signals 
// redirects inputs if previous pipe
// redirects output if next command exists 
static void	child_pipe_handler(t_command *command, int pipe_fd, int fd[2])
{
	child_signals();
	if (pipe_fd != -1)
	{
		if (dup2(pipe_fd, STDIN_FILENO) == -1)
			perror("dupfailSTDIN");
		close(pipe_fd);
	}
	if (command->next)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			perror("dupfailSTDOUT");
		close(fd[1]);
	}
}

// pipes if next command
// forks and sends to child functions
// handles parent's fd's
// pipe_fd stores the previous fd
// if pipe_fd = -1, there's no previous pipe
static int	command_loop(t_command *command, t_shell *shell)
{
	static int		pipe_fd = -1;
	pid_t			pid;
	int				fd[2];

	ignore_signals();
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
	return (pid);
}

// main execution loops through commands:
// if command is single built_in without redirections, executes built_in
// else command_loop forks and returns childs pid
// pids are stored into an array and used in wait_kids
void	execution(t_shell *shell, t_command	*command)
{
	int			*pids;
	int			i;
	t_command	*cmd_head;

	pids = arena_alloc(shell->arena, sizeof(int) * (cmd_count(command)));
	if (!pids)
		return ;
	i = 0;
	cmd_head = command;
	while (command)
	{
		if (command->argv)
		{
			if (is_built_in(command) == true && command->position != LAST
				&& !command->next && !command->infile && !command->outfile)
				execute_built_in(command, shell);
			else
			{
				pids[i] = command_loop(command, shell);
				i++;
			}
		}
		command = command->next;
	}
	wait_kids(shell, pids, i, cmd_head);
}
