/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:56:42 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/09/11 12:52:43 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

void	child_sigint(int sig)
{
	(void)sig;
	g_sigint = false;
}


void	child(t_command *command, t_shell *shell)
{
	char	*path;
	char	**env_array;

	if (prepare_files(command) != 0)
			exit(EXIT_FAILURE);
	path = find_command_path(command, shell);
	if (!path)
		exit(EXIT_FAILURE);
	env_array = env_to_array(shell);
	ft_putstr_fd("HELLO FROM THE KID\n", 2);
	if (execve(path, command->argv, env_array) == -1)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

void command_exit_status(t_shell *shell, pid_t pid)
{
	int child_status;
	
	waitpid(pid, &child_status, 0);
	if (WEXITSTATUS(child_status))
		shell->exit_status = WEXITSTATUS(child_status);
	else if (WIFSIGNALED(child_status))
	{
		int sig = WTERMSIG(child_status);
		shell->exit_status = sig + 128;
	}
}

void	command_loop(t_command *command, t_shell *shell)
{
	pid_t	pid;
	static int		pipe_fd = -1;
	int	fd[2];
	
	if (command->next)
		pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, child_sigint);
		if (pipe_fd != -1)
		{
			if (dup2(pipe_fd, STDIN_FILENO) == -1) // redirect in to open pipe_fd
				perror("dupfailSTDIN");
			close(pipe_fd);
		}
		if (command->next)//pipe is not open but there's next
		{
			close(fd[0]);
			if (dup2(fd[1], STDOUT_FILENO) == -1) //redirect out to next pipe
				perror("dupfailSTDOUT");
			close(fd[1]);
		}
		child(command, shell);
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
}

void	execution(t_shell *shell, t_command	*command)
{

	while (command)
	{
		if (command->argv)
		{
			if (check_if_built_in(command) == true && !command->next && !command->infile && !command->outfile)
				execute_built_in(command, shell);
			command_loop(command, shell);
		}
		command = command->next;
	}
	return ;
}
	

/*
1. exit codes
2. heredoc
4. refactor, arena split
5. error, exitcodes
6. leaks and open fd with echo hello >> test1.txt | wc -l 
----------------------

Exit Code	Meaning	Example command	Explanation
0	Success	ls	Command runs successfully
1	General error	grep "nomatch" file.txt	No lines matched (grep returns 1 for no matches)
2	Misuse of shell builtins or syntax error	cd --unknownoption or test with invalid syntax	Shell builtin misused or syntax error
126	Command invoked but not executable	chmod -x script.sh; ./script.sh	File exists but is not executable
127	Command not found	nocommand	Command does not exist
128 + n	Command terminated by signal n	Run command and send signal, e.g. sleep 10 then Ctrl-C (SIGINT, 2)	Exit status = 128 + 2 = 130 (interrupted by SIGINT)

----------------------

*/
