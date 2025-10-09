#include "minishell.h"

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

int	exit_arguments(t_command *command)
{
	if (exit_isdigit(command->argv[1]) == 1)
	{
		if (!command->argv[2])
			return (ft_atoi(command->argv[1]));
		else
		{
			printf("exit: too many arguments\n");
			return (-1);
		}
	}
	else
		printf("exit: %s: numeric argument required\n", command->argv[1]);
	return (0);
}

void	exit_builtin(t_shell *shell, t_command *command)
{
	int	e;

	e = 0;
	if (command->argv[1])
		e = exit_arguments(command);
	if (e == -1)
		return ;
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
