/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:18:06 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/19 12:22:22 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

#include "minishell.h"
#include "arena.h"

typedef struct s_env
{
	char	*name;
	char	*value;
	int		expanded;
	t_env	*next;
}t_env;


typedef struct s_shell
{
	int		exit_status;
	t_env	*env_list;
}t_shell;

void	execution(mem_arena arena, t_command *command);
void	*init_shell(mem_arena arena, t_shell *shell);
void	*init_env_list(mem_arena arena);

void	check_if_builtin(t_command *command, t_shell *shell);

int		echo(t_command *command);
int		cd(char *next_cmd);
int 	pwd(char *cmd);

#endif