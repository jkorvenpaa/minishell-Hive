/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:44:09 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/19 11:36:07 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Ensures that old file names are freed before setting a new one.
 * It duplicates the provided value to avoid using token memory directly.
 */
static void	assign_redirect_file(char **field, const char *value)
{
	if (*field)
		free(*field);
	*field = ft_strdup(value);
}

/**
 * Processes a redirection token and updates the command.
 * Depending on the type, updates infile, outfile, append flag or heredoc.
 */
static void	handle_redirection(t_command *cmd, t_token *redir_token)
{
	if (redir_token->type == REDIRECT_IN)
		assign_redirect_file(&cmd->infile, redir_token->next->value);
	else if (redir_token->type == REDIRECT_OUT)
	{
		assign_redirect_file(&cmd->outfile, redir_token->next->value);
		cmd->append = 0;
	}
	else if(redir_token->type == APPEND)
	{
		assign_redirect_file(&cmd->outfile, redir_token->next->value);
		cmd->append = 1;
	}
	else if(redir_token->type == HEREDOC)
		assign_redirect_file(&cmd->heredoc, redir_token->next->value);
}

/**
 * Creates a new command node if no current command exists,
 * then adds the token value to the comand's argv array.
 */
static void	handle_word_token(t_command **cmd_list, t_command **current_cmd, t_token *token)
{
	if (!*current_cmd)
	{
		*current_cmd = create_command_node();
		append_command_to_list(cmd_list, *current_cmd);
	}
	add_argument_to_argv(*current_cmd, token->value);
}

/**
 * Processes a redirection operator and its target.
 * Creates a new command node if needed, then calls handle_redirection().
 */
static void	handle_redirection_token(t_command **cmd_list, t_command **current_cmd, t_token *token)
{
	if (!*current_cmd)
	{
		*current_cmd = create_command_node();
		append_command_to_list(cmd_list, *current_cmd);
	}
	handle_redirection(*current_cmd, token);
}

/**
 * Groups tokens into a linked list of commands.
 * Iterates over tokens and builds command nodes with arguments and redirections.
 * Handles pipes by resetting the current command pointer, starting a new one.
 * Returns the head of the linked list of parsed commands.
 */
t_command	*group_commands(t_token *tokens)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_token	*token_iterator;

	cmd_list = NULL;
	current_cmd = NULL;
	token_iterator = tokens;

	while (token_iterator)
	{
		if (token_iterator->type == WORD)
			handle_word_token(&cmd_list, &current_cmd, token_iterator);
		else if (token_iterator->type == REDIRECT_IN || token_iterator->type == REDIRECT_OUT
			|| token_iterator->type == APPEND || token_iterator->type == HEREDOC)
		{
			handle_redirection_token(&cmd_list, &current_cmd, token_iterator);
			token_iterator = token_iterator->next;
		}
		else if (token_iterator->type == PIPE)
			current_cmd = NULL;
		token_iterator = token_iterator->next;
	}
	return (cmd_list);
}
