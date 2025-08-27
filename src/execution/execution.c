/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:56:42 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/27 15:36:07 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

void	command_loop(t_command *command, t_shell *shell)
{
	//pid_t	pid;
	//char	*path;
	char *const	*env;
	env = (char *const *)shell->env_list;

	if (check_if_built_in(command) == true)
	{
		if (!command->next) //&& !command->prev)
			execute_built_in(command, shell);
	}
	else
	{
		//pid = fork();
		//if (pid == 0)
		{
			//prepare_files(command);
			//if (check_if_built_in(command) == true)
				//execute_built_in(command, shell);
			//path = find_command_path(command, shell);
			//if (execve(path, command->argv, env) == -1)
				//return;//exit_built_in();
		}
		//waitpid
	}
}

void	execution(t_shell *shell, t_command	*command_list)
{
	//init_history
	// if(!shell) ??
	while (command_list)
	{
		command_loop(command_list, shell);
		command_list = command_list->next;
	}
	//free_arena(shell->arena);
	return ;
}
/*
loop user inputs;
{
	command validation, action, history.
	after executing command:
		Free arenas used for tokens, commands.
	Loop back to prompt.
}
free arenas for env and history only in exit

*/

/*
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


*/
