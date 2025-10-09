#include "minishell.h"

// prints env list
int	env_builtin(t_shell *shell, char *next_cmd)
{
	t_env	*temp;

	if (next_cmd)
		return (0);
	temp = shell->env_list;
	while (temp)
	{
		printf("%s=%s\n", temp->name, temp->value);
		temp = temp->next;
	}
	return (0);
}

// second loop of bubble sort
static int	sort_loop(t_shell *shell, int swap, t_env *node, t_env *prev)
{
	t_env	*temp;
	int		len;

	while (node->next)
	{
		len = ft_strlen((node->name + 1));
		if (ft_strncmp(node->name, node->next->name, len) > 0)
		{
			temp = node->next;
			node->next = temp->next;
			temp->next = node;
			if (prev)
				prev->next = temp;
			else
				shell->env_list = temp;
			prev = temp;
			swap++;
		}
		else
		{
			prev = node;
			node = node->next;
		}
	}
	return (swap);
}

// first loop of bubble sort for alphabetical env
void	sort_env(t_shell *shell)
{
	int		swap;
	t_env	*node;
	t_env	*prev;

	swap = -1;
	while (swap != 0)
	{
		swap = 0;
		node = shell->env_list;
		prev = NULL;
		swap = sort_loop(shell, swap, node, prev);
	}
}
