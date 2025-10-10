/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:11:46 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/10 11:12:33 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// update env value
static t_env	*update_env(t_env *new, t_shell *shell, char *next_cmd)
{
	char	*equal;
	int		i;
	int		e;

	i = ft_strlen(next_cmd);
	equal = ft_strchr(next_cmd, '=');
	e = equal - next_cmd;
	new->value = a_substr(shell->env_arena, next_cmd, e + 1, i - e);
	if (!new->value)
		return (NULL);
	return (new);
}

// create new env node
static t_env	*new_env(t_env *new, t_shell *shell, char *next_cmd)
{
	t_env	*temp;
	char	*equal;
	int		i;
	int		e;

	temp = shell->env_list;
	i = ft_strlen(next_cmd);
	equal = ft_strchr(next_cmd, '=');
	e = equal - next_cmd;
	new = arena_alloc(shell->env_arena, sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = a_substr(shell->env_arena, next_cmd, 0, e);
	if (!new->name)
		return (NULL);
	new->value = a_substr(shell->env_arena, next_cmd, e + 1, i - e);
	if (!new->value)
		new->value = arena_strdup(shell->env_arena, "");
	new->next = NULL;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return (new);
}

// export contains only alphanumeric or _
static int	valid_export_name(char *next_cmd)
{
	int	i;

	i = 0;
	if (ft_strchr(next_cmd, '=') == NULL)
		return (1);
	while (next_cmd[i] != '=')
	{
		if (ft_isalnum(next_cmd[i]) == 0 && next_cmd[i] != '_')
			return (1);
		else
			i++;
	}
	return (0);
}

//checks update an env node or create a new node
static int	export_values(t_command *cmd, t_shell *shell)
{
	t_env	*new;
	int		i;
	char	*name;
	int		len;

	i = 1;
	while (cmd->argv[i])
	{
		len = ft_strchr(cmd->argv[i], '=') - cmd->argv[i];
		name = a_substr(shell->arena, cmd->argv[i], 0, len);
		if (valid_export_name(cmd->argv[i]) == 1)
			return (1);
		new = get_env_node(shell->env_list, name);
		if (new == NULL)
			new = new_env(new, shell, cmd->argv[i]);
		else
			new = update_env(new, shell, cmd->argv[i]);
		if (!new)
			return (1);
		i++;
	}
	return (0);
}

// prints the env_list sorted if no argv[1], else exports the value
int	export(t_command *cmd, t_shell *shell)
{
	t_env	*temp;

	if (!cmd->argv[1] && shell->env_list)
	{
		sort_env(shell);
		temp = shell->env_list;
		while (temp)
		{
			printf("declare -x %s=\"%s\"\n", temp->name, temp->value);
			temp = temp->next;
		}
		return (0);
	}
	else
		return (export_values(cmd, shell));
}
