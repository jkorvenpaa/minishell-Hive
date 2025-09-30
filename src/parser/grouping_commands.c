/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:44:09 by nmascaro          #+#    #+#             */
/*   Updated: 2025/09/30 15:56:47 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Processes a redirection token and updates the command.
 * Depending on the type, updates infile, outfile, append flag or heredoc.
 */
static void	handle_redirection(mem_arena *arena, t_command *cmd, t_token *redir_token)
{
	if (redir_token->type == RED_IN)
		cmd->infile = arena_strdup(arena, redir_token->next->value);
	else if (redir_token->type == RED_OUT)
	{
		outfile_to_list(arena, cmd, redir_token->next->value);
		cmd->outfile = arena_strdup(arena, redir_token->next->value);
		cmd->append = 0;
	}
	else if (redir_token->type == APPEND)
	{
		outfile_to_list(arena, cmd, redir_token->next->value);
		cmd->outfile = arena_strdup(arena, redir_token->next->value);
		cmd->append = 1;
	}
	else if (redir_token->type == HEREDOC)
	{
		cmd->heredoc_quoted = redir_token->next->was_quoted;
		cmd->heredoc = arena_strdup(arena, redir_token->next->value);
	}
}

/**
 * Creates a new command node if no current command exists,
 * then adds the token value to the comand's argv array.
 */
static void	handle_word_token(mem_arena *arena, t_command **cmd_list, t_command **current_cmd, t_token *token)
{
	if (!*current_cmd)
	{
		*current_cmd = create_command_node(arena);
		append_command_to_list(cmd_list, *current_cmd);
	}
	add_argument_to_argv(arena, *current_cmd, token->value);
}

/**
 * Processes a redirection operator and its target.
 * Creates a new command node if needed, then calls handle_redirection().
 */
static void	handle_redirection_token(mem_arena *arena, t_command **cmd_list, t_command **current_cmd, t_token *token)
{
	if (!*current_cmd)
	{
		*current_cmd = create_command_node(arena);
		append_command_to_list(cmd_list, *current_cmd);
	}
	handle_redirection(arena, *current_cmd, token);
}

/**
 * Groups tokens into a linked list of commands.
 * Iterates over tokens and builds command nodes with arguments and redirections.
 * Handles pipes by resetting the current command pointer, starting a new one.
 * Returns the head of the linked list of parsed commands.
 */
t_command	*group_commands(mem_arena *arena, t_token *tokens)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_token		*token_iterator;

	cmd_list = NULL;
	current_cmd = NULL;
	token_iterator = tokens;
	while (token_iterator)
	{
		if (token_iterator->type == WORD)
			handle_word_token(arena, &cmd_list, &current_cmd, token_iterator);
		else if (token_iterator->type == RED_IN || token_iterator->type == RED_OUT
			|| token_iterator->type == APPEND || token_iterator->type == HEREDOC)
		{
			handle_redirection_token(arena, &cmd_list, &current_cmd, token_iterator);
			token_iterator = token_iterator->next;
		}
		else if (token_iterator->type == PIPE)
			current_cmd = NULL;
		token_iterator = token_iterator->next;
	}
	return (cmd_list);
}
