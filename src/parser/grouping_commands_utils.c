/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:49:33 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/15 15:00:16 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Allocates and initializes a new command struct.
 * Returns a pointer to the allocated command or NULL on failure.
 */
t_command	*create_command_node(void)
{
	t_command	*node;

	node = malloc(sizeof(t_command)); // TODO: MALLOC
	if (!node)
		return (NULL);
	node->argv = NULL;
	node->infile  = NULL;
	node->outfile = NULL;
	node->append = 0;
	node->heredoc = NULL;
	node->next = NULL;
	return (node);
}

/**
 * Adds a string argument to a command's argv array.
 * Creates a new argv array one element larger (plus '\0'), copies old arguments,
 * duplicates the new argument, and frees the old argv array.
 * Returns the new argv array pointer or NULL on failure.
 */
void	*add_argument_to_argv(t_command *cmd, char *arg)
{
	int	count;
	int	i;
	char	**new_argv;

	count = 0;
	if (cmd->argv)
	{
		while (cmd->argv[count])
			count++;
	}
	new_argv = malloc(sizeof(char *) * (count +  2)); // TODO: MALLOC
	if (!new_argv)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[count] = ft_strdup(arg);
	new_argv[count + 1] = NULL;
	free(cmd->argv);
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
