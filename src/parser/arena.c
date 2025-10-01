/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 15:37:05 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/01 15:37:45 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//EXIT PROGRAM GRACEFULLY IF NULL;

//init area before using, returns arena
t_mem_arena	*arena_init(void)
{
	t_mem_arena	*arena;

	arena = malloc(sizeof(t_mem_arena));
	if (!arena)
		return (NULL);
	arena->size = A_SIZE;
	arena->buffer = malloc(arena->size);
	if (!arena->buffer)
	{
		free (arena);
		return (NULL);
	}
	arena->offset = 0;
	arena->next = NULL;
	return (arena);
}

//returns a pointer to arena where, alloc starts.
//Moves offset for the amount of bytes reserved
void	*arena_alloc(t_mem_arena *arena, size_t bytes)
{
	void	*ptr;

	if (arena->offset + bytes > arena->size)
	{
		arena->next = arena_init();
		if (!arena->next)
			return (NULL);
		return (arena_alloc(arena->next, bytes));
	}
	ptr = arena->buffer + arena->offset;
	arena->offset = arena->offset + bytes;
	return (ptr);
}

void	free_arena(t_mem_arena *arena)
{
	t_mem_arena	*temp;

	while (arena)
	{
		temp = arena->next;
		if (arena->buffer)
		{
			free (arena->buffer);
			arena->buffer = NULL;
		}
		free (arena);
		arena = temp;
	}
	arena = NULL;
}

void	arena_reset(t_mem_arena *arena)
{
	while (arena)
	{
		arena->offset = 0;
		arena = arena->next;
	}
}
