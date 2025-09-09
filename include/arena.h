/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:29:31 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/09/09 14:19:19 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ARENA_H
# define ARENA_H

# include "minishell.h"

#  define A_SIZE (32 * 1024) // 32 KB

typedef struct t_mem_arena
{
	char* 		buffer; //pointer to start of arena
	size_t 		size;    // size of whole arena
	size_t 		offset; // tracking the start of free space
	struct t_mem_arena	*next; //pointer to next arena
} mem_arena;

void	*arena_alloc(mem_arena *arena, size_t bytes);
mem_arena	*arena_init();
void	arena_reset(mem_arena *arena);
void	free_arena(mem_arena *arena);
char	*arena_strdup(mem_arena *arena, const char *str);
char	*ar_substr(mem_arena *arena, char const *s, unsigned int st, size_t len);
char	*ar_strjoin(mem_arena *arena, char const *s1, char const *s2);
char	*arena_itoa(mem_arena *env_arena, int n);

#endif