/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:18:06 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/21 17:02:01 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

#include "minishell.h"
#include "arena.h"
#include <stdbool.h>


//this struct should move to parsing
typedef struct s_env
{
	char	*name;// NAME
	char	*value;// nuria
	int		expanded;// 0 or 1
	t_env	*next; //
}t_env;

typedef struct s_history
{
	char 	*history; //command made
	int		nbr; //number of  history commands
	char	*next;
}t_history;


typedef struct s_shell
{
	int			exit_status; // $? fetches this
	mem_arena	*arena;
	t_env		*env_list;
//	t_history	*history_list;
}t_shell;

void	execution(mem_arena *arena, t_command *command, t_env *env);
void	*init_shell(mem_arena *arena, t_shell *shell, t_env *env);
//void	*init_env_list(mem_arena arena);

void	check_if_builtin(t_command *command, t_shell *shell);

int		echo(t_command *command);
int		cd(char *next_cmd);
int 	pwd(char *cmd);

int		export(char	*next_cmd, t_shell *shell);
t_env	*new_env(t_env *new, t_shell *shell, char *next_cmd);
t_env	*update_env(t_env *new, t_shell *shell, char *next_cmd);


#endif