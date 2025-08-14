/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:44:09 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/14 15:55:47 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// IN PROCCESS! Need to: add files to makefile, add comments, add functions in header file, and add this logic to the rest of code
#include "minishell.h"

static void	assign_redirect_file(char **field, const char *value)
{
	if (*field)
		free(*field);
	*field = ft_strdup(value);
}
static void	handle_redirection(t_command *cmd, t_token *redir_token)
{
	if (!redir_token->next)
		return ; // syntax error here: missing filename
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
static void	handle_word_token(t_command **cmd_list, t_command **current_cmd, t_token *token)
{
	if (!*current_cmd)
	{
		*current_cmd = create_command_node();
		append_command_to_list(cmd_list, *current_cmd);
	}
	add_argument_to_argv(*current_cmd, token->value);
}
static void	handle_redirection_token(t_command **cmd_list, t_command **current_cmd, t_token *token)
{
	if (!*current_cmd)
	{
		*current_cmd = create_command_node();
		append_command_to_list(cmd_list, *current_cmd);
	}
	handle_redirection(*current_cmd, token);
}
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
