
#include "minishell.h"

/**
 * Adds a new environment node to the list. If the list is empty,
 * the new node becomes the head. Otherwise, it's appended to the
 * end of the list.
 */
static void	append_env_to_list(t_env **head, t_env *new_node)
{
	t_env	*temp;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;	
}

/**
 * Fills a t_env node with a name and value from a string.
 * Splits "NAME=VALUE" into name and value and if '=' isn't found,
 * the variable is created with an empty value.
 * Returns 1 on success, 0 if allocation failed.
 */
static int	process_env_variable(mem_arena *env_arena, t_env *new_env, const char *env_str)
{
	char *equal_sign;

	equal_sign = ft_strchr(env_str, '='); //search for = and set pointer there
	if (equal_sign)
	{
		new_env->name = ar_substr(env_arena, env_str, 0, equal_sign - env_str); //before = (p.ex: USER)
		new_env->value = arena_strdup(env_arena, equal_sign + 1); // after = (p.ex.: guest) copies everything after equal sign
	}
	else //if no value is assigned, the environment variable still exists but just with empty value
	{
		new_env->name = arena_strdup(env_arena, env_str);
		new_env->value = arena_strdup(env_arena, "");
	}
	if (!new_env->name || !new_env->value)
		return (0);
	return (1);
}

/**
 * Builds a linked list of environment variables from envp.
 * Iterates through envp, creates a t_env node for each string
 * and appends to the list.
 * Returns head of the linked list or NULL or error.
 */
t_env	*init_env_list(mem_arena *env_arena, const char **envp)
{
	int	i;
	t_env *head;
	t_env *new_env;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		new_env = arena_alloc(env_arena, sizeof(t_env));
		if (!new_env)
			return (NULL);
		ft_bzero(new_env, sizeof(t_env)); //zero all fields of t_env
		if (!process_env_variable(env_arena, new_env, envp[i]))
			return (NULL);
		new_env->next = NULL;
		append_env_to_list(&head, new_env);
		i++;
	}
	return (head);
}

/**
 * Finds an environment variable node by name.
 * Compares each node's name with the given one until an exact match is found.
 * Returns matching node or NULL if not found.
 */
t_env	*get_env_node(t_env *env_list, const char *name)
{
	size_t	name_len;

	if (!name)
		return (NULL);
	name_len = ft_strlen(name);
	while (env_list)
	{
		if (ft_strncmp(env_list->name, name, name_len) == 0
		&& env_list->name[name_len] == '\0') // second check ensures exact match
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}
