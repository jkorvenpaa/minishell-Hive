/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:29:31 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/22 09:57:39 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ARENA_H
# define ARENA_H

# include "minishell.h"

#  define A_SIZE (64 * 1024) // 64 KB

typedef struct t_mem_arena
{
	char*  buffer; //pointer to start of arena
	size_t size;    // size of whole arena
	size_t offset; // tracking the start of free space
} mem_arena;

void	*arena_alloc(mem_arena *arena, size_t bytes);
mem_arena	*arena_init();

#endif