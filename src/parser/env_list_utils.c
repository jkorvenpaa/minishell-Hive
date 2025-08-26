
#include "minishell.h"

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
static int	process_env_variable(mem_arena *env_arena, t_env *new_env, char *env_str)
{
	char *equal_sign;

	equal_sign = ft_strchr(env_str, '='); //search for = and set pointer there
	if (equal_sign)
	{
		new_env->name = ar_substr(env_arena, env_str, 0, equal_sign - env_str); //before = (p.ex: USER) copies before (but its always set to before the =))
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
t_env	*init_env_list(mem_arena *env_arena, char **envp)
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
