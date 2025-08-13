/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:44:09 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/13 16:08:04 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// IN PROCCESS! 
#include "minishell.h"

t_command	*create_command_node(void)
{

}
void	*add_argument_to_argv(t_command *cmd, char *arg)
{

}
void	handle_redirection(t_command *cmd, t_token *redir_token)
{

}
void	append_command_to_list(t_command **head, t_command *new_cmd)
{
	
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
		{
			if (!current_cmd)
			{
				current_cmd = create_command_node();
				append_command_to_list(&cmd_list, current_cmd);
			}
			add_argument_to_argv(current_cmd, token_iterator->value);
		}
		else if (token_iterator->type == REDIRECT_IN || token_iterator->type == REDIRECT_OUT
			|| token_iterator->type == APPEND || token_iterator->type == HEREDOC)
		{
			if (!current_cmd)
			{
				current_cmd = create_command_node();
				append_command_to_list(&cmd_list, current_cmd);
			}
			handle_redirection(current_cmd, token_iterator);
			token_iterator = token_iterator->next;
		}
		else if (token_iterator->type == PIPE)
		{

		}
		token_iterator = token_iterator->next;
	}
	return (cmd_list);
}



