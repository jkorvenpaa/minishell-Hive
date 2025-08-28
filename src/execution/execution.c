/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:56:42 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/28 16:30:06 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

void	command_loop(t_command *command, t_shell *shell)
{
	pid_t	pid;
	char	*path;
	char **env_array;

	if (check_if_built_in(command) == true)
	{
		if (!command->next && !command->infile && !command->outfile) //heredoc??&& !command->prev)
			execute_built_in(command, shell);
		else
		{
			pid = fork();
			if (pid == 0)
			{
				if (prepare_files(command) != 0)
					exit(EXIT_FAILURE);
				execute_built_in(command, shell);
				exit(EXIT_SUCCESS);
			}
			else if (pid > 0)
				waitpid(pid, NULL, 0);
		}
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			if (prepare_files(command) != 0)
				exit(EXIT_FAILURE);
			path = find_command_path(command, shell);
			//printf("%s\n", path);

			//env_array = env_to_array(env_arena, shell->env_list);
			execve(path, command->argv, env_array);
			exit(EXIT_SUCCESS);
			//if (execve(path, command->argv, env) == -1)
			//	return;//exit_built_in();
		}
	else if (pid > 0)
		waitpid(pid, NULL, 0);
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
	//free_arena(shell->arena);
	return ;
}

/*

1. Arena split and strjoin
2. env_to_array function
3. outfile + heredoc
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
