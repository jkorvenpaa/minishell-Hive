/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_ins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:12:47 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/10 11:12:51 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_built_in(t_command *command)
{
	char	*cmd;

	cmd = command->argv[0];
	if (ft_strncmp(cmd, "echo", 4) == 0 && ft_strlen(cmd) == 4)
		return (true);
	else if (ft_strncmp(cmd, "cd", 2) == 0 && ft_strlen(cmd) == 2)
		return (true);
	else if (ft_strncmp(cmd, "pwd", 3) == 0 && ft_strlen(cmd) == 3)
		return (true);
	else if (ft_strncmp(cmd, "export", 6) == 0 && ft_strlen(cmd) == 6)
		return (true);
	else if (ft_strncmp(cmd, "unset", 5) == 0 && ft_strlen(cmd) == 5)
		return (true);
	else if (ft_strncmp(cmd, "env", 3) == 0 && ft_strlen(cmd) == 3)
		return (true);
	else if (ft_strncmp(cmd, "exit", 4) == 0 && ft_strlen(cmd) == 4)
		return (true);
	return (false);
}

void	execute_built_in(t_command *command, t_shell *shell)
{
	char	*cmd;
	char	*next_cmd;

	cmd = command->argv[0];
	next_cmd = command->argv[1];
	if (ft_strncmp(cmd, "echo", 5) == 0)
		shell->exit_status = echo(command);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		shell->exit_status = cd(shell, next_cmd);
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		shell->exit_status = pwd(shell);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		shell->exit_status = export(command, shell);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		shell->exit_status = unset(next_cmd, shell);
	else if (ft_strncmp(cmd, "env", 3) == 0)
		shell->exit_status = env_builtin(shell, next_cmd);
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		exit_builtin(shell, command);
}
