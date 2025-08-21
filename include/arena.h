/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:29:31 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/21 16:13:55 by jkorvenp         ###   ########.fr       */
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
	mem_arena	*next //pointer to next arena
} mem_arena;

#endif