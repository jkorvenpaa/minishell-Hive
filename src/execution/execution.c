/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:56:42 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/09/09 14:00:40 by jkorvenp         ###   ########.fr       */
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


	path = find_command_path(command, shell);
	env_array = env_to_array(shell);
	ft_putstr_fd("HELLO FROM THE KID\n", 2);
	if (execve(path, command->argv, env_array) == -1)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}


void	command_loop(t_command *command, t_shell *shell)
{
	pid_t	pid;
	static int		pipe_fd = -1;
	int	fd[2];
	
	if (!command->argv)
		return ;
	if (check_if_built_in(command) == true && !command->next && !command->infile && !command->outfile)
	{
		execute_built_in(command, shell);
		return ;
	}
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
		if (prepare_files(command) != 0)
			exit(EXIT_FAILURE);
		child(command, shell);
		exit(EXIT_SUCCESS);
	}
	else
	{	
		int child_status;
		waitpid(pid, &child_status, 0);
		WEXITSTATUS(child_status);
			
		if (pipe_fd != -1)
		{
			close(pipe_fd);
		}
		if (command->next)
		{
			close(fd[1]);
			pipe_fd = fd[0];
		}
		else
            pipe_fd = -1;
	}

}

void	execution(t_shell *shell, t_command	*command_list)
{
	//init_history
	while (command_list)
	{
		command_loop(command_list, shell);
		command_list = command_list->next;
	}
	return ;
}
	

/*
1. pipelines!!
2. heredoc
3. history
4. refactor, arena split
5. error, exitcodes
----------------------

HISTORYYYYY

----------------------
if readline return NULL = ctrl+D was pushed and exit_builtin should be called

If external command:

Open (Redirection Files) 🔹 Uses open() to prepare files for <, >, >>, <<

Pipe 🔹 Uses pipe() to create a tube between commands if | is found

Fork 🔹 Uses fork() to create a child process for each command
in chid handle ctrlC = sigint, parent sleeps/ignores signals mean while

Dup2 🔹 Uses dup2() to connect input/output to the right place (file or pipe)

Execve 🔹 Uses execve() to run the actual command (like cat, grep, etc.)

Close 🔹 Uses close() to shut unused pipe ends and file descriptors

Waitpid 🔹 Uses waitpid() to wait for all child processes to finish

	after executing command:
		Free arenas used for tokens, commands.

free arenas for env and history only in exit


*/
