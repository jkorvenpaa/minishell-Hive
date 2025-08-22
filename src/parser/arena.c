
#include "minishell.h"

//EXIT PROGRAM GRACEFULLY IF NULL;

//init area before using, returns arena
mem_arena	*arena_init(void)
{
	mem_arena	*arena;

	arena = malloc(sizeof(mem_arena));
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
	return (arena);
}

//returns a pointer to arena where, alloc starts.
//Moves offset for the amouth of bytes reserved
void	*arena_alloc(mem_arena *arena, size_t bytes)
{
	void	*ptr;

	if (arena->offset + bytes > arena->size)
	{
		arena->next = arena_init();
		if (!arena->next)
			return (NULL);
		arena_alloc(arena->next, bytes);
	}
	ptr = arena->buffer + arena->offset;
	arena->offset = arena->offset + bytes;
	return (ptr);
}

void	free_arena(mem_arena *arena)
{
	mem_arena	*temp;

	while (arena)
	{
		temp = arena->next;
		free (arena->buffer);
		free (arena);
		arena = temp;
	}
}
