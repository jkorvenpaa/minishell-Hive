/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:29:31 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/15 11:37:50 by jkorvenp         ###   ########.fr       */
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

#endif