/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:44:09 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/01 16:52:49 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Processes a redirection token and updates the command.
 * Depending on the type, updates infile, outfile, append flag or heredoc.
 */
static void	handle_redir(t_mem_arena *ar, t_command *cmd, t_token *redir_token)
{
	if (redir_token->type == RED_IN)
		cmd->infile = arena_strdup(ar, redir_token->next->value);
	else if (redir_token->type == RED_OUT)
	{
		outfile_to_list(ar, cmd, redir_token->next->value);
		cmd->outfile = arena_strdup(ar, redir_token->next->value);
		cmd->append = 0;
	}
	else if (redir_token->type == APPEND)
	{
		outfile_to_list(ar, cmd, redir_token->next->value);
		cmd->outfile = arena_strdup(ar, redir_token->next->value);
		cmd->append = 1;
	}
	else if (redir_token->type == HEREDOC)
	{
		cmd->heredoc_quoted = redir_token->next->was_quoted;
		cmd->heredoc = arena_strdup(ar, redir_token->next->value);
	}
}

/**
 * Creates a new command node if no current command exists,
 * then adds the token value to the comand's argv array.
 */
static void	wd_tk(t_mem_arena *a, t_command **lst, t_command **cmd, t_token *tk)
{
	if (!*cmd)
	{
		*cmd = create_command_node(a);
		append_command_to_list(lst, *cmd);
	}
	add_argument_to_argv(a, *cmd, tk->value);
}

/**
 * Processes a redirection operator and its target.
 * Creates a new command node if needed, then calls handle_redirection().
 */
static void	rdr_tk(t_mem_arena *a, t_command **lst, t_command **cmd, t_token *t)
{
	if (!*cmd)
	{
		*cmd = create_command_node(a);
		append_command_to_list(lst, *cmd);
	}
	handle_redir(a, *cmd, t);
}
static void	command_position(t_command *head)
{
	int	count;
	t_command	*current;
	int	i;

	count = 0;
	current = head;
	while (current)
	{
		count++;
		current = current->next;
	}
	current = head;
	i = 0;
	while (current)
	{
		if (i == 0)
			current->position = FIRST;
		else if (i == count - 1)
			current->position = LAST;
		else
			current->position = MIDDLE;
		current = current->next;
		i++;
	}	
}
/**
 * Groups tokens into a linked list of commands.
 * Iterates over tokens and builds command nodes with arguments and redirections.
 * Handles pipes by resetting the current command pointer, starting a new one.
 * Returns the head of the linked list of parsed commands.
 */
t_command	*group_commands(t_mem_arena *arena, t_token *tokens)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_token		*token_iter;

	cmd_list = NULL;
	current_cmd = NULL;
	token_iter = tokens;
	while (token_iter)
	{
		if (token_iter->type == WORD)
			wd_tk(arena, &cmd_list, &current_cmd, token_iter);
		else if (token_iter->type == RED_IN || token_iter->type == RED_OUT
			|| token_iter->type == APPEND || token_iter->type == HEREDOC)
		{
			rdr_tk(arena, &cmd_list, &current_cmd, token_iter);
			token_iter = token_iter->next;
		}
		else if (token_iter->type == PIPE)
			current_cmd = NULL;
		token_iter = token_iter->next;
	}
	command_position(cmd_list);
	return (cmd_list);
}
