/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:18:06 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/27 11:25:28 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"

//this struct should move to parsing
typedef struct s_env
{
	char	*name;// NAME
	char	*value;// nuria
	struct s_env	*next; //
}	t_env;

typedef struct s_history
{
	char 	*history; //command made
	int		nbr; //number of  history commands
	struct s_history	*next;
}	t_history;


typedef struct s_shell
{
	int			exit_status; // $? fetches this
	mem_arena	*arena;
	mem_arena	*env_arena;
	t_env		*env_list;
//	t_history	*history_list;
}	t_shell;


/*
void	execution(t_shell *shell, t_command	*command_list);

void	check_if_builtin(t_command *command, t_shell *shell);

int		echo(t_command *command);
int		cd(char *next_cmd);
int 	pwd(char *cmd);

int		export(char	*next_cmd, t_shell *shell);
t_env	*new_env(t_env *new, t_shell *shell, char *next_cmd);
t_env	*update_env(t_env *new, t_shell *shell, char *next_cmd);
int		unset(char *next_cmd, t_shell *shell);
int	env_builtin(t_shell *shell);

*/


#endif