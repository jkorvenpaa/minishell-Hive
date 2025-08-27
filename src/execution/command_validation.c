/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_validation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:43:04 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/27 12:43:45 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

bool	check_if_built_in(t_command *command)
{
	char	*cmd;

	cmd = command->argv[0];
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (true);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		return (true);
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (true);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (true);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (true);
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (true);
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		return (true);
	return (false);
}

void	execute_built_in(t_command *command, t_shell *shell)
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
	//else if (ft_strncmp(cmd, "exit", 4) == 0)
	//	exit_builtin(shell);
}

char	*find_command_path(t_command *command, t_shell *shell)
{
	char	*path;
	char	**folder;
	char	*final_path;
	int		i;
	int		len;

	i = 0;
	path = getenv("PATH");
	folder = ft_split(path, ':');///modify to arena_split
	if (!folder)
		return (NULL); //+mem_ error
	while (folder[i])
	{
		len = ft_strlen(folder[i] + 1 + ft_strlen(command->argv[0]) + 1);
		final_path = arena_alloc(shell->arena, len);
		if (!final_path)
			return (NULL); //+mem_error
		if (access(final_path, X_OK) == 0)
			return (final_path);//found the command path
		i++;
	}
	return (NULL);//path not found
}

void	prepare_files(t_command	*command) // t_shell *shell
{
	int	fd;

	if (command->outfile)
	{
		fd = open(command->outfile, O_RDONLY);
		if (fd < 0)
		{
			// erno + exit
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	//if (command->heredoc) need to be added
	if (command->infile)
	{
		if (command->append == 1)
			open(command->infile, O_APPEND);
		else
			open(command->infile, O_RDWR);
		if (fd < 0)
		{
			// erno + exit
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}
