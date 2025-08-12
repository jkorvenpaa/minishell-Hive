/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:10:40 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/12 16:00:20 by nmascaro         ###   ########.fr       */
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

int	main(int argc, char *argv[], char *const envp[])
{
	(void)argc; //Not sure if we need it
	(void)argv; // Not sure if we need it
	(void)envp; //Not using now
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

