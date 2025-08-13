/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 10:19:48 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/13 11:52:54 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_tokens(t_token *head) // for testing only!!!!
{
    while (head)
    {
        printf("Token: %s Type: %d\n", head->value, head->type);
        head = head->next;
    }
}

void	run_parser(void) //"main" for parsing part
{
	char	*input;
	t_token	*tokens;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input) //When readline() returns NULL (we press ctrl + D)
		{
			printf("exit\n");
			break;
		}
		if (ft_strncmp(input, "exit", 5) == 0)
		{
			free(input);
			break;
		}
		tokens = tokenize_input(input);
		if (!tokens)
		{
			printf("Error: failed to parse input\n");
			free(input);
		}
		else
		{
			print_tokens(tokens); // here for testing only!!!
			free_token_list(tokens);
			free(input);
		}
	}
	return (0);
}