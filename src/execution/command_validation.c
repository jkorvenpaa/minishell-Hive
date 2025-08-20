/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_validation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:43:04 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/20 15:53:35 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	check_if_builtin(t_command *command, t_shell *shell)
{
	char	*cmd;
	char	*next_cmd;

	cmd = command->argv[0];
	next_cmd = command->argv[1];
	if (ft_strncmp(cmd, "echo", 4) == 0)
		shell->exit_status = echo(command);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		shell->exit_status = cd(next_cmd);
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		shell->exit_status = pwd(cmd);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		shell->exit_status = export(next_cmd, shell);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		shell->exit_status = unset(next_cmd, shell);
	else if (ft_strncmp(cmd, "env", 3) == 0)
		shell->exit_status = env_builtin(shell);
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		exit_builtin(shell);
		
}

/*If external command:

Search in PATH.

Fork a child process.

Set up redirections and pipes.

Use execve() to run the command.

Wait for child processes to finish.




Free memory (tokens, AST, env list if needed).
Loop back to prompt.
*/

/*
	else if (ft_strncmp(cmd, "export", 6) == 0)
	{
		//export NAME=value 
		//->$NAME = value
		//save the env list here, remember to trim quotes away
	}
	else if (ft_strncmp(cmd, "unset", 5) == 0)
	{
		//modify env list
	}
	else if (ft_strncmp(cmd, "env", 3) == 0)
	{
		//print created env list
		
		while (env)
		{
			printf("%s=%s", ENV, value);
		}
			
	}
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		/*
		if (!next_cmd)
		free
		close fd
		kill background process
		exit status = $?
			
		
}
*/
// define paths for others