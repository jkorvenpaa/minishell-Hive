/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:43:28 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/13 16:17:38 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*  validate input and create history

valid command?
	Command: '' not found

unclosed pipes or quotes?

Save only valid input to history?
	history file location home?: ~/.minishell_history
	save after each valid
*/
#include "minishell.h"

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
