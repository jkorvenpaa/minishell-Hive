#include "minishell.h"
#include "execution.h"

int	env_builtin(t_shell *shell)
{
	t_env	*temp;

	temp = shell->env_list;
	while (temp)
	{
		printf("%s=%s\n", temp->name, temp->value);
		temp = temp->next;
	}
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

t_env	*update_env(t_env *new, t_shell *shell, char *next_cmd)
{
	char	*equal;
	int		i;
	int		e;

	i = ft_strlen(next_cmd);
	equal = ft_strchr(next_cmd, '=');
	e = equal - next_cmd; //position of = sign
	new->value = ar_substr(shell->env_arena, next_cmd, e + 1, i - e);
	if (!new->value)
		return (NULL);
	return (new);
}

t_env	*new_env(t_env *new, t_shell *shell, char *next_cmd)
{
	t_env	*temp;
	char	*equal;
	int		i;
	int		e;

	temp = shell->env_list;
	i = ft_strlen(next_cmd);
	equal = ft_strchr(next_cmd, '=');
	e = equal - next_cmd; //position of = sign
	new = arena_alloc(shell->env_arena, sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = ar_substr(shell->env_arena, next_cmd, 0, e);
	if (!new->name)
		return (NULL);
	new->value = ar_substr(shell->env_arena, next_cmd, e + 1, i - e);
	if (!new->value)
		return (NULL);
	new->next = NULL;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return (new);
}

int	export(char	*next_cmd, t_shell *shell)
{
	t_env	*temp;
	t_env	*new;

	temp = shell->env_list;
	if (!next_cmd) // just print the list
	{
		while (temp)
		{
			printf("declare -x %s=\"%s\"\n", temp->name, temp->value);
			temp = temp->next;
		}
		return (0);
	}
	if (ft_strchr(next_cmd, '=') == NULL) // invalid input for export
		return (1);
	new = get_env_node(shell->env_list, next_cmd); // if already in env list
	if (new == NULL)
		new = new_env(new, shell, next_cmd); //if not make a new
	else
		new = update_env(new, shell, next_cmd); // if yes, update value
	if (!new)
		return (1);
	return (0);
}
