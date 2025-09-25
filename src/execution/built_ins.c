/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:50:23 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/09/25 13:14:50 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

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
		if (command->argv[i+1])
			printf(" ");
		i++;
	}
	if (ft_strncmp(command->argv[1], "-n", 2) != 0)
		printf("\n");
	return (0);
}

int	cd(char *next_cmd)
{
	if (!next_cmd)
		next_cmd = getenv("HOME");
	if (chdir(next_cmd) != 0)
	{
		printf("cd: %s: No such file or directory\n", next_cmd); //check message
		return (1); //?? check the status
	}
	return (0);
}

int	pwd(void)
{
	char	*cmd;

	cmd = getcwd(NULL, 0);
	if (!cmd)
		return (1); //check status
	printf("%s\n", cmd);
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

void	exit_builtin(t_shell *shell)
{
	int	e;

	e = shell->exit_status;
	printf("exit\n");
	free_arena(shell->env_arena);
	if (shell->arena)
		free_arena(shell->arena);
	free(shell);
	exit(e);
}
