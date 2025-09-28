/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_validation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:43:04 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/09/28 14:08:43 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

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
	else if (ft_strncmp(cmd, "export", 6) == 0&& ft_strlen(cmd) == 6)
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
		shell->exit_status = pwd();
	else if (ft_strncmp(cmd, "export", 6) == 0)
		shell->exit_status = export(command, shell);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		shell->exit_status = unset(next_cmd, shell);
	else if (ft_strncmp(cmd, "env", 3) == 0)
		shell->exit_status = env_builtin(shell, next_cmd);
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		exit_builtin(shell, command);
}

char	*find_command_path(t_command *command, t_shell *shell)
{
	char	*path;
	char	**folder;
	char	*final_path;
	int		i;

	if (ft_strchr(command->argv[0], '/'))
		return (arena_strdup(shell->arena, command->argv[0]));
	i = 0;
	path = getenv("PATH");
	folder = ar_split(shell->arena, path, ':');
	if (!folder)
		return (NULL);
	while (folder[i])
	{
		final_path = ar_strjoin(shell->arena, folder[i], "/");
		if (!final_path)
			return (NULL);
		final_path = ar_strjoin(shell->arena, final_path, command->argv[0]);
		if (!final_path)
			return (NULL);
		if (access(final_path, X_OK) == 0)
			return (final_path);//found the command path
		i++;
	}
	return (NULL);//path not found
}

int	env_len(t_env *env_list)
{
	int		len;
	t_env	*temp;

	temp = env_list;
	len = 0;
	while (temp)
	{
		temp = temp->next;
		len++;
	}
	return (len);
}

char	**env_to_array(t_shell *shell)
{
	char	**env_array;
	char	*temp;
	t_env	*head;
	int		i;
	int		len;

	i = 0;
	head = shell->env_list;
	len = env_len(shell->env_list);
	env_array = arena_alloc(shell->arena, len + 1);
	while (head)
	{
		temp = ar_strjoin(shell->arena, head->name, "=");
		env_array[i] = ar_strjoin(shell->arena, temp, head->value);
		i++;
		head = head->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
