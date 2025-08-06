/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:16:46 by nmascaro          #+#    #+#             */
/*   Updated: 2025/08/06 15:48:53 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*split_commands(char *input)
{
	int	i;
	char	**words;
	t_token	*head;
	t_token	*new;

	words = ft_split(input, ' ');
	if (!words)
		return (NULL);
	head = NULL;
	i = 0;
	while (words[i])
	{
		new = new_node(words[i]);
		if (!new)
		{
			free_list_nodes(head);
			free_split(words);
			return (NULL);
		}
		add_to_end(&head, new);
		i++;
	}
	free_split(words);
	return (head);
}
