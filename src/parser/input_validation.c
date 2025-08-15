/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:43:28 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/15 15:00:56 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*  validate input and create history

valid command?
	Command: '' not found

unclosed pipes or quotes?

Save only valid input to history?
	history file location home?: ~/.minishell_history
	save after each valid
*/

void	valid_command()
{
	

}

bool	valid_token(t_token	**list)
{
	char *token;
	t_token node;

	token = (*list)->value;
	//add loop through list
	
	if ((*list)->type = WORD)
	{
		
		if (token[0] == '"'|| token[0] == '\'')
		{
			if (token[ft_strlen(token)] == '"' || '\'')
				return (true);
			return (false);
		}
		else valid_command()
	}
}
