/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:49:33 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/01 16:40:53 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Allocates and initializes a new command struct.
 * Returns a pointer to the allocated command or NULL on failure.
 */
t_command	*create_command_node(t_mem_arena *arena)
{
	t_command	*node;

	node = arena_alloc(arena, sizeof(t_command));
	if (!node)
		return (NULL);
	node->argv = NULL;
	node->infile = NULL;
	node->outfile_list = NULL;
	node->outfile = NULL;
	node->append = 0;
	node->heredoc = NULL;
	node->heredoc_quoted = 0;
	node->position = MIDDLE; 
	node->next = NULL;
	return (node);
}

/**
 * Adds a string argument to a command's argv array.
 * Creates a new argv array one element larger (plus '\0'), copies old arguments,
 * and duplicates the new argument.
 * Returns the new argv array pointer or NULL on failure.
 */
void	*add_argument_to_argv(t_mem_arena *arena, t_command *cmd, char *arg)
{
	int		count;
	int		i;
	char	**new_argv;

	count = 0;
	if (cmd->argv)
	{
		while (cmd->argv[count])
			count++;
	}
	new_argv = arena_alloc(arena, sizeof(char *) * (count + 2));
	if (!new_argv)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[count] = arena_strdup(arena, arg);
	if (!new_argv[count])
		return (NULL);
	new_argv[count + 1] = NULL;
	cmd->argv = new_argv;
	return (new_argv);
}

/**
 * Adds a command node to the end of the list.
 * If the list is empty, sets head to new_cmd,
 * if not, traverses to the end and attaches new_cmd there.
 */
void	append_command_to_list(t_command **head, t_command *new_cmd)
{
	t_command	*temp;

	if (!*head)
	{
		*head = new_cmd;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_cmd;
}

void	outfile_to_list(t_mem_arena *arena, t_command *cmd, const char *name)
{
	int		count;
	int		i;
	char	**list;

	count = 0;
	if (cmd->outfile_list)
	{
		while (cmd->outfile_list[count])
			count++;
	}
	list = arena_alloc(arena, sizeof(char *) * (count + 2));
	if (!list)
		return ;
	i = 0;
	while (i < count)
	{
		list[i] = cmd->outfile_list[i];
		i++;
	}
	list[count] = arena_strdup(arena, name);
	list[count + 1] = NULL;
	cmd->outfile_list = list;
}
