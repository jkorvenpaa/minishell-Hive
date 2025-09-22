#include "minishell.h"
#include "execution.h"

int	env_builtin(t_shell *shell, char *next_cmd)
{
	t_env	*temp;

	if (next_cmd)
		return(0);
	sort_env(shell);
	temp = shell->env_list;
	while (temp)
	{
		printf("%s=%s\n", temp->name, temp->value);
		temp = temp->next;
	}
	return (0);
}

static int	sort_loop(t_shell *shell, int swap, t_env *node, t_env *prev)
{
	t_env	*temp;

	while (node->next)
	{
		if (ft_strncmp(node->name, node->next->name, ft_strlen(node->name)) > 0)
		{
			temp = node->next;// new first
			node->next = temp->next;//move the firstpoint to point to seconds next
			temp->next = node; //move the secondpoint to point to first
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
