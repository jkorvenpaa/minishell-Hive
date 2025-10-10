/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:29:31 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/10/10 11:09:50 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARENA_H
# define ARENA_H

# include "minishell.h"

# define A_SIZE 32768

typedef struct s_mem_arena
{
	char				*buffer; //pointer to start of arena
	size_t				size; // size of whole arena
	size_t				offset; // tracking the start of free space
	struct s_mem_arena	*next; //pointer to next arena
}	t_mem_arena;

void		*arena_alloc(t_mem_arena *arena, size_t bytes);
t_mem_arena	*arena_init(void);
void		arena_reset(t_mem_arena *arena);
void		free_arena(t_mem_arena *arena);
char		*arena_strdup(t_mem_arena *arena, const char *str);
char		*a_substr(t_mem_arena *a, char const *s, unsigned int st, size_t l);
char		*ar_strjoin(t_mem_arena *arena, char const *s1, char const *s2);
char		*arena_itoa(t_mem_arena *env_arena, int n);

#endif