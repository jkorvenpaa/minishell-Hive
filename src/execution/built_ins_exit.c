/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:11:35 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/16 10:59:45 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exit_isdigit(char *num)
{
	int	i;

	i = 0;
	if (num[i] == '-' || num[i] == '+')
		i++;
	if (!num[i] || num[i] == '-' || num[i] == '+')
		return (0);
	while (num[i])
	{
		if (ft_isdigit(num[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

static int	exit_arguments(t_command *command)
{
	int			overflow;
	long long	num;

	num = 0;
	overflow = 0;
	if (exit_isdigit(command->argv[1]) == 1)
	{
		if (ft_strncmp(command->argv[1], "-9223372036854775808", 21) == 0)
			return (0);
		num = ft_atol(command->argv[1], &overflow);
		if (!command->argv[2] && !overflow)
			return ((unsigned char)num);
		else if (!overflow)
		{
			printf("exit: too many arguments\n");
			return (-1);
		}
	}
	printf("exit: %s: numeric argument required\n", command->argv[1]);
	return (2);
}

void	exit_builtin(t_shell *shell, t_command *command)
{
	int	e;

	e = 0;
	if (command->argv[1])
		e = exit_arguments(command);
	else
		e = shell->exit_status;
	if (e == -1)
	{
		shell->exit_status = 1;
		return ;
	}
	printf("exit\n");
	close (shell->fd_in);
	free_arena(shell->env_arena);
	if (shell->arena)
		free_arena(shell->arena);
	free(shell);
	exit(e);
}
