
#include "execution.h"

int	env_builtin(t_shell shell)
{
	return (0);
}

int unset(char *next_cmd, t_shell shell)
{
	return (0);
}

bool	env_exists()
{
	while(temp)
	{
		if (ft_strncmp(temp->name, next_cmd, ) == 0)
		unset();
	}

}

int	new_env(t_shell *shell, char *next_cmd)
{
	t_env	*temp;
	t_env *new;
	int	i;
	int	j;

	temp = shell->env_list;
	i = 0;
	j = 0;
	new = arena_alloc(shell->arena, sizeof(t_env));
	while (next_cmd)
	{
		if (j == 0 && next_cmd[i] == '=')
			j = i;
		i++;
	}
	new->name = arena_alloc(shell->arena, j);
	ft_strlcpy(new->name, next_cmd, j);
	//if (!new->name)
	new->value = arena_alloc(shell->arena, i-j);
	ft_strlcpy(new->name, next_cmd, i-j);
	//if (!new->value)
	new->expanded = 1;
	new->next = NULL;
	while(temp->next)
		temp = temp->next;
	temp->next = new;
	return (0);
}

int	export(char	*next_cmd, t_shell *shell)
{
	t_env	*temp;

	temp = shell->env_list;
	if (!next_cmd)
	{
		while(temp)
		{
			printf("declare -x %s=\"%s\"\n", temp->name, temp->value);
			temp = temp->next;
		}
		return(0);
	}
	if(ft_strchr(next_cmd, '=') == NULL)
		return (1);
	env_exists(shell->env_list, next_cmd) == true;
		return(0); 
	if (new_env(shell, next_cmd) != 0)
		return(1);
	else
		return(0);
}
