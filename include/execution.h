/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:18:06 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/10/10 11:00:10 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <fcntl.h>
# include <unistd.h> 
# include <stdbool.h>
# include <sys/wait.h>
# include <errno.h>
# include <signal.h>

typedef struct s_shell		t_shell;
typedef struct s_command	t_command;
typedef struct s_env		t_env;

//execution
void	execution(t_shell *shell, t_command	*command_list);
void    clean_child(t_shell *shell);

//signals
void	init_signals(void);
void	heredoc_signals(void);
void	child_signals(void);
void	ignore_signals(void);
void	hd_exit_signals(void);

// Execution_utils
void	command_exit_status(t_shell *shell, pid_t last_pid);
void	execve_error(char *command_name);
void	command_error(t_shell *shell, char *command_name);
int		cmd_count(t_command *command);
void	wait_kids(t_shell *shell, int *pids, int count, t_command *command);

bool	is_built_in(t_command *command);
void	execute_built_in(t_command *command, t_shell *shell);
char	*find_command_path(t_command *command, t_shell *shell);
char	**env_to_array(t_shell *shell);
char	**arena_split(t_shell *shell, char const *s, char c);

// Built-ins
int		echo(t_command *command);
int		cd(t_shell *shell, char	*next);
int		pwd(t_shell *shell);
void	exit_builtin(t_shell *shell, t_command *command);

int		export(t_command *cmd, t_shell *shell);
int		unset(char *next_cmd, t_shell *shell);
int		env_builtin(t_shell *shell, char *next_cmd);
void	sort_env(t_shell *shell);

// Prepare files
int		prepare_files(t_command	*command);

// Heredocs
int		handle_heredoc(t_shell *shell, t_command *command);
void	unlink_infile(t_command *command);

#endif