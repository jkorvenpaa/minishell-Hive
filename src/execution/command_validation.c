/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_validation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:43:04 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/22 13:11:40 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
# include "execution.h"

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

//if readline return NULL = ctrl+D was pushed and exit_builtin should be called

/*If external command:

Search in PATH.

Fork a child process.

in chid handle ctrlC = sigint, parent sleeps/ignores signals mean while

Set up redirections and pipes.

Use execve() to run the command.

Wait for child processes to finish.

----------------------

HISTORYYYYY

----------------------


*/