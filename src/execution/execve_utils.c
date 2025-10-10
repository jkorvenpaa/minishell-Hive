#include "minishell.h"

// loops folders and checks if access == 0: the command is found
// else returns NULL
static char	*path_loop(t_shell *shell, t_command *command, char **folder)
{
	char	*final_path;
	int		i;

	i = 0;
	while (folder[i])
	{
		final_path = ar_strjoin(shell->arena, folder[i], "/");
		if (!final_path)
			return (NULL);
		final_path = ar_strjoin(shell->arena, final_path, command->argv[0]);
		if (!final_path)
			return (NULL);
		if (access(final_path, X_OK) == 0)
			return (final_path);
		i++;
	}
	return (NULL);
}

// command path finder:
// finds path from env_list, splits the path to **folder (delimeter : )
char	*find_command_path(t_command *command, t_shell *shell)
{
	t_env	*node;
	char	**folder;
	char	*final_path;

	if (ft_strchr(command->argv[0], '/'))
		return (arena_strdup(shell->arena, command->argv[0]));
	node = get_env_node(shell->env_list, "PATH");
	if (!node || !node->value)
		return (NULL);
	folder = arena_split(shell, node->value, ':');
	if (!folder)
		return (NULL);
	final_path = path_loop(shell, command, folder);
	return (final_path);
}

static int	env_len(t_env *env_list)
{
	int		len;
	t_env	*temp;

	temp = env_list;
	len = 0;
	while (temp)
	{
		temp = temp->next;
		len++;
	}
	return (len);
}

// transforms env from linked list to **array for execve 
char	**env_to_array(t_shell *shell)
{
	char	**env_array;
	char	*temp;
	t_env	*head;
	int		i;
	int		len;

	i = 0;
	head = shell->env_list;
	len = env_len(shell->env_list);
	env_array = arena_alloc(shell->arena, sizeof(char *) * (len + 1));
	if (!env_array)
		return (NULL);
	while (head)
	{
		temp = ar_strjoin(shell->arena, head->name, "=");
		env_array[i] = ar_strjoin(shell->arena, temp, head->value);
		i++;
		head = head->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
