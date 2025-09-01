/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:50:23 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/09/01 16:26:11 by jkorvenp         ###   ########.fr       */
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
	if (ft_strncmp(command->argv[1], "-n", 2) == 0)
		i = 2;
	else
		i = 1;
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
		printf(" cd: %s: No such file or directory", next_cmd); //check message
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

void	exit_builtin(t_shell *shell)
{
	int e;

	e = shell->exit_status;
	printf("exit\n");
	free_arena(shell->env_arena);
	free_arena(shell->arena);
	free(shell);
	exit(e);
}

/*
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		
		if (!next_cmd)
		free
		close fd
		kill background process
		exit status = $?
			
		*/