/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:50:23 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/10/07 10:21:36 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(t_command *command)
{
	int	i;

	if (!command->argv[1])
	{
		printf("\n");
		return (0);
	}
	i = 1;
	while (ft_strncmp(command->argv[i], "-n", 2) == 0)
		i++;
	while (command->argv[i])
	{
		printf("%s", command->argv[i]);
		if (command->argv[i + 1])
			printf(" ");
		i++;
	}
	if (ft_strncmp(command->argv[1], "-n", 2) != 0)
		printf("\n");
	return (0);
}

static void	get_working_directory(t_shell *shell, const char *path)
{
	char	*cwd;
	char	*new_path;
	char	*temp;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		shell->working_dir = arena_strdup(shell->env_arena, cwd);
		free (cwd);
		return ;
	}
	if (shell->working_dir)
		printf("cd: error retrieving current directory: getcwd: "
			"cannot access parent directories: "
			"No such file or directory\n");
	if (path[0] == '/')
		new_path = arena_strdup(shell->env_arena, path);
	else
	{
		temp = ar_strjoin(shell->env_arena, shell->working_dir, "/");
		new_path = ar_strjoin(shell->env_arena, temp, path);
	}
	shell->working_dir = new_path;
}

int	cd(t_shell *shell, char	*next)
{
	t_env	*temp;
	char	*dir;

	if (!next)
	{
		temp = get_env_node(shell->env_list, "HOME");
		if (!temp)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
		dir = temp->value;
	}
	else
		dir = next;
	if (chdir(dir) != 0)
	{
		printf("cd: %s: No such file or directory\n", next);
		return (1);
	}
	get_working_directory(shell, dir);
	return (0);
}

int	pwd(t_shell *shell)
{
	char	*cmd;

	cmd = getcwd(NULL, 0);
	if (!cmd)
	{
		if (shell->working_dir)
		{
			printf("%s\n", shell->working_dir);
			return (0);
		}
		return (1); //check status
	}
	printf("%s\n", cmd);
	free(cmd); // getcwd allocates memory (same as readline)
	return (0);
}

int	unset(char *next_cmd, t_shell *shell)
{
	t_env	*node;
	t_env	*temp;

	node = get_env_node(shell->env_list, next_cmd);
	if (!node)
		return (0);
	temp = shell->env_list;
	if (temp == node)
	{
		shell->env_list = temp->next;
		node = ft_memset(node, 0, sizeof(t_env));
		return (0);
	}
	while (temp->next != node)
		temp = temp->next;
	temp->next = node->next;
	node = ft_memset(node, 0, sizeof(t_env));
	return (0);
}

int	exit_isdigit(char *num)
{
	int	i;

	i = 0;
	while (num[i] == '-' || num[i] == '+')
		i++;
	while (num[i])
	{
		if (ft_isdigit(num[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

void	exit_builtin(t_shell *shell, t_command *command)
{
	int	e;

	e = 0;
	if (command->argv[1])
	{
		if (exit_isdigit(command->argv[1]) == 1)
		{
			if (!command->argv[2])
				e = ft_atoi(command->argv[1]);
			else
			{
				printf("exit: too many arguments\n");
				return ;
			}
		}
		else
			printf("exit: %s: numeric argument required\n", command->argv[1]);
	}
	if (e == 0)
		e = shell->exit_status;
	printf("exit\n");
	close (shell->fd_in);
	free_arena(shell->env_arena);
	if (shell->arena)
		free_arena(shell->arena);
	free(shell);
	exit(e);
}
