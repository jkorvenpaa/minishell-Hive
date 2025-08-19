/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:50:23 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/19 12:25:48 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	echo(t_command *command)
{
	int	i;

	if (ft_strncmp(command->argv[1], "-n", 2) == 0)
		i = 2;
	else
		i = 1;
	while (command->argv[i])
	{
		printf("%s", command->argv[i]);
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

int	pwd(char *cmd)
{
	cmd = getcwd(NULL, 0);
	if (!cmd)
		return (1); //check status
	printf("%s\n", cmd);
	return (0);
}

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
			
		*/